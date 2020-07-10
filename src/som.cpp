#include "../include/som.h"
#include <stdio.h>

void *funcion_hilo_seleccion(void *funcion)
{
    parametrosHIlo param = *(parametrosHIlo*)funcion;
    int indice = param.indice_hilo;
    SOM *som1 = param.som1;
    int indice_neurona;
    while(!som1->terminoEntrenarse){
        if(som1->inicio_calculo_hilos && som1->hilodeSeleccion[indice].estado_calculo_finalizado==false){
            double distancia = 0;
            double distanciaAux = std::numeric_limits<double>::infinity();
            int indiceNeuronaGanadora = 0;
            for(int indiceNeu=som1->hilodeSeleccion[indice].limite_inferior; indiceNeu<som1->hilodeSeleccion[indice].limite_superior; indiceNeu++)
            {
                Arreglos::getNeurona(som1->hilodeSeleccion[indice].neurona, som1->redNeuronal, indiceNeu);
                distancia = Distancias::distanciaEuclidea_1(som1->entrada, som1->hilodeSeleccion[indice].neurona);//distanciaEuclidea_1(entrada, neurona);

                if(distancia < distanciaAux)
                {
                    distanciaAux = distancia;
                    indiceNeuronaGanadora = indiceNeu;
                }
            }
            som1->hilodeSeleccion[indice].valor_neurona_ganadora=distanciaAux;
            som1->hilodeSeleccion[indice].estado_calculo_finalizado=true;
            som1->hilodeSeleccion[indice].indice_neurona_ganadora=indiceNeuronaGanadora;
        }
    }
    printf("termino del hilo [%d]\n",indice);
    pthread_exit(NULL);
}

static void iniciarMapa(bool **marcasMapa)
{
    for(int fila=0; fila<Configuracion::ANCHO; fila++)
    {
        for(int columna=0; columna<Configuracion::LARGO; columna++)
        {
            marcasMapa[fila][columna] = false;
        }
    }
}

SOM::SOM(double **datos)
{
    pausarEntrenamiento = false;
    terminoEntrenarse = false;

    mapaHex = new NeuronaHex*[Configuracion::ANCHO];
    for(int i=0; i<Configuracion::ANCHO; i++)
        mapaHex[i] = new NeuronaHex[Configuracion::LARGO];

    redNeuronal=new double*[Configuracion::NUMERO_ENTRADAS];
    for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
        redNeuronal[i] = new double[Configuracion::NUMERO_NEURONAS];

    marcasMapa = new bool*[Configuracion::ANCHO];
    for(int i=0; i<Configuracion::ANCHO; i++)
        marcasMapa[i] = new bool[Configuracion::LARGO];

    numeroEntradas = Configuracion::NUMERO_ENTRADAS;
    numeroNeuronas = Configuracion::NUMERO_NEURONAS;
    rangoVecindad = Configuracion::RANGO_VECINDAD;
    numeroIteraciones = alfa/beta;
    iteracion = 0;
    crearMatrizConexionHex(mapaHex);
    datosEntrenamiento = datos;


    for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
    {
        alfas[i] = alfa;
        betas[i] = beta;
    }
    pesosAleatorios();
    ciclos = 0;

    if(Configuracion::NUMERO_HILOS>1){
        int tam_seccion = Configuracion::NUMERO_NEURONAS/Configuracion::NUMERO_HILOS;
        int inf = 0;
        for(int i=0; i<Configuracion::NUMERO_HILOS; i++){
            HiloSeleccion h;
            h.incide_hilo=i;
            h.limite_inferior=inf;
            h.limite_superior=(inf+tam_seccion<Configuracion::NUMERO_NEURONAS)?inf+tam_seccion:
                Configuracion::NUMERO_NEURONAS;
            h.entrada= new double[Configuracion::NUMERO_ENTRADAS];
            h.neurona= new double[Configuracion::NUMERO_ENTRADAS];
            inf+=tam_seccion;
            hilodeSeleccion.push_back(h);
        }
    }
}

SOM::~SOM()
{
    //dtor
    /**Liberando memoria de la matriz marcasMapa*/
    for(int i=0; i<Configuracion::ANCHO; i++)
        delete[] marcasMapa[i];
    delete []marcasMapa;

    for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
        delete[] redNeuronal[i];
    delete []redNeuronal;

    for(int i=0; i<Configuracion::ANCHO; i++)
        delete[] mapaHex[i];
    delete []mapaHex;

    for(int i=0; i<Configuracion::NUMERO_HILOS; i++)
    {
        delete hilodeSeleccion[i].entrada;
        delete hilodeSeleccion[i].neurona;
    }

    terminoEntrenarse = false;
    hilodeSeleccion.clear();


}

/**-------------------------------------------------------------------------------*/


/**
funciones de olvido para la tasa de aprendizaje de la red neuronal
*/
//olvido lineal: beta es una constante que degrada alfa(taza de aprendizaje) hasta llegar a 0
void SOM::olvidoProgresivo(double *alfa, double beta)
{
    *alfa = *alfa-beta;
}

void SOM::olvidoLogaritmico(double *alfa, double alfa0,double ciclo)
{
    *alfa = alfa0*exp(-4*ciclo);

}
/**-------------------------------------------------------------------------------*/

double SOM::aprendizajeHebb(double alfa, double distanciaVecindario, double e, double u)
{
    return (alfa/(double)distanciaVecindario)*(e- u);
}

/**actualizando lo peso de la neurona*/
void SOM::actualizarPesosNeurona(int distanciaVecin, int indiceNeurona)
{
    for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
    {
        redNeuronal[i][indiceNeurona]+=aprendizajeHebb(alfas[i], distanciaVecin, entrada[i], redNeuronal[i][indiceNeurona]);
        if(redNeuronal[i][indiceNeurona] < 0.00001)//problema de numero infinitamente chicos
            redNeuronal[i][indiceNeurona] = 0;
    }
}

void SOM::propagacionAprendizaje(int distanciaVecin, int fila, int columna)
{
    NeuronaHex *neurona = &(mapaHex[fila][columna]);
    double listaNeurona[6];
    bool listaBool[6];

    if(distanciaVecin <= Configuracion::RANGO_VECINDAD)
    {
        if(!marcasMapa[neurona->lado1.fila][neurona->lado1.columna])
        {
            listaBool[0] = true;
            listaNeurona[0] = Configuracion::LARGO*neurona->lado1.fila+neurona->lado1.columna;
        }
        else
            listaBool[0] = false;

        if(!marcasMapa[neurona->lado2.fila][neurona->lado2.columna])
        {
            listaBool[1] = true;
            listaNeurona[1] = Configuracion::LARGO*neurona->lado2.fila+neurona->lado2.columna;
        }
        else
            listaBool[1] = false;

        if(!marcasMapa[neurona->lado3.fila][neurona->lado3.columna])
        {
            listaBool[2] = true;
            listaNeurona[2] = Configuracion::LARGO*neurona->lado3.fila+neurona->lado3.columna;
        }
        else
            listaBool[2] = false;

        if(!marcasMapa[neurona->lado4.fila][neurona->lado4.columna])
        {
            listaBool[3] = true;
            listaNeurona[3] = Configuracion::LARGO*neurona->lado4.fila+neurona->lado4.columna;
        }
        else
            listaBool[3] = false;

        if(!marcasMapa[neurona->lado5.fila][neurona->lado5.columna])
        {
            listaBool[4] = true;
            listaNeurona[4] = Configuracion::LARGO*neurona->lado5.fila+neurona->lado5.columna;
        }
        else
            listaBool[4] = false;

        if(!marcasMapa[neurona->lado6.fila][neurona->lado6.columna])
        {
            listaBool[5] = true;
            listaNeurona[5] = Configuracion::LARGO*neurona->lado6.fila+neurona->lado6.columna;
        }
        else
            listaBool[5] = false;
        marcasMapa[neurona->lado1.fila][neurona->lado1.columna] = true;
        marcasMapa[neurona->lado2.fila][neurona->lado2.columna] = true;
        marcasMapa[neurona->lado3.fila][neurona->lado3.columna] = true;
        marcasMapa[neurona->lado4.fila][neurona->lado4.columna] = true;
        marcasMapa[neurona->lado5.fila][neurona->lado5.columna] = true;
        marcasMapa[neurona->lado6.fila][neurona->lado6.columna] = true;
        int indiceNeurona;

        for(int i_vecino=0; i_vecino<6; i_vecino++)
        {
            indiceNeurona = listaNeurona[i_vecino];
            if(listaBool[i_vecino]==true)
            {
                actualizarPesosNeurona(distanciaVecin, indiceNeurona);
            }

        }


        if(listaBool[0])
        {
            mapaHex[neurona->lado1.fila][neurona->lado1.columna].numero_activaciones+=(1/(double)(distanciaVecin+1));
            propagacionAprendizaje(distanciaVecin+1, neurona->lado1.fila, neurona->lado1.columna);
        }

        if(listaBool[1])
        {
            mapaHex[neurona->lado2.fila][neurona->lado2.columna].numero_activaciones+=(1/(double)(distanciaVecin+1));
            propagacionAprendizaje(distanciaVecin+1, neurona->lado2.fila, neurona->lado2.columna);
        }

        if(listaBool[2])
        {
            mapaHex[neurona->lado3.fila][neurona->lado3.columna].numero_activaciones+=(1/(double)(distanciaVecin+1));
            propagacionAprendizaje(distanciaVecin+1, neurona->lado3.fila, neurona->lado3.columna);
        }

        if(listaBool[3])
        {
            mapaHex[neurona->lado4.fila][neurona->lado4.columna].numero_activaciones+=(1/(double)(distanciaVecin+1));
            propagacionAprendizaje(distanciaVecin+1, neurona->lado4.fila, neurona->lado4.columna);
        }

        if(listaBool[4])
        {
            mapaHex[neurona->lado5.fila][neurona->lado5.columna].numero_activaciones+=(1/(double)(distanciaVecin+1));
            propagacionAprendizaje(distanciaVecin+1, neurona->lado5.fila, neurona->lado5.columna);
        }

        if(listaBool[5])
        {
            mapaHex[neurona->lado6.fila][neurona->lado6.columna].numero_activaciones+=(1/(double)(distanciaVecin+1));
            propagacionAprendizaje(distanciaVecin+1, neurona->lado6.fila, neurona->lado6.columna);
        }



    }
}

void SOM::aprendizaje(int indiceNeurona)
{
    iniciarMapa(marcasMapa);
    int fila = indiceNeurona/Configuracion::LARGO;
    int columna = indiceNeurona%Configuracion::LARGO;

    mapaHex[fila][columna].numero_activaciones+=1;
    marcasMapa[fila][columna] = true;
    actualizarPesosNeurona(1, indiceNeurona);
    propagacionAprendizaje(2, fila, columna);
}

void SOM::pesosAleatorios()
{
    for(int i=0; i<numeroNeuronas; i++)
    {
        for(int j=0; j<numeroEntradas; j++)
        {
            redNeuronal[j][i]=i;//Arreglos::fRand(0, 1);
        }
    }
}

int SOM::seleccionNeuronaGanadora()
{
    double distancia = 0;
    double distanciaAux = std::numeric_limits<double>::infinity();
    int indiceNeuronaGanadora = 0;
    for(int indiceNeu=0; indiceNeu<numeroNeuronas; indiceNeu++)
    {
        Arreglos::getNeurona(neurona, redNeuronal, indiceNeu);
        distancia = Distancias::distanciaEuclidea_1(entrada, neurona);//distanciaEuclidea_1(entrada, neurona);

        if(distancia < distanciaAux)
        {
            distanciaAux = distancia;
            indiceNeuronaGanadora = indiceNeu;
        }
    }
    return indiceNeuronaGanadora;
}

int SOM::seleccionNeuronaGanadoraHilo(int indiceHilo)
{
    double distancia = 0;
    double distanciaAux = std::numeric_limits<double>::infinity();
    int indiceNeuronaGanadora = 0;
    for(int indiceNeu=hilodeSeleccion[indiceHilo].limite_inferior; indiceNeu<hilodeSeleccion[indiceHilo].limite_superior; indiceNeu++)
    {
        Arreglos::getNeurona(hilodeSeleccion[indiceHilo].neurona, redNeuronal, indiceNeu);
        distancia = Distancias::distanciaEuclidea_1(entrada, hilodeSeleccion[indiceHilo].neurona);//distanciaEuclidea_1(entrada, neurona);

        if(distancia < distanciaAux)
        {
            distanciaAux = distancia;
            indiceNeuronaGanadora = indiceNeu;
        }
    }
    hilodeSeleccion[indiceHilo].valor_neurona_ganadora=distanciaAux;
    return indiceNeuronaGanadora;
}

void SOM::ejemplo1()
{
    pesosAleatorios();
    int contador = 0;
    for(int columna=0; columna<Configuracion::NUMERO_NEURONAS; columna++)
    {
        printf("%d: ", contador);
        for(int fila=0; fila<Configuracion::NUMERO_ENTRADAS; fila++)
        {
            printf("| %f ", redNeuronal[fila][columna]);
        }
        contador+=1;
        printf("|\n");
    }
    Arreglos::getFila(entrada, datosEntrenamiento, 0);
    aprendizaje(20);
    printf("|\n");
    printf("|\n");
    printf("|\n");
    contador = 0;
    for(int columna=0; columna<Configuracion::NUMERO_NEURONAS; columna++)
    {
        printf("%d: ", contador);
        for(int fila=0; fila<Configuracion::NUMERO_ENTRADAS; fila++)
        {
            printf("| %f ", redNeuronal[fila][columna]);
        }
        contador+=1;
        printf("|\n");
    }
    //FicheroRNA::escribirJS(Configuracion::ANCHO, Configuracion::LARGO, mapaHex, redNeuronal);
}

void SOM::entrenamiento()
{
    terminoEntrenarse=false;
    if(Configuracion::NUMERO_HILOS>1){
        parametrosHIlo *param;
         for(int i=0; i<Configuracion::NUMERO_HILOS; i++){
            param = new parametrosHIlo();
            param->indice_hilo=i;
            param->som1=this;
            pthread_create(&hilodeSeleccion[i].hilo, NULL, &funcion_hilo_seleccion, param);
         }
    }
    printf("numero iter: %d\n", numeroIteraciones);
    bool flag_olvido_progresivo = Configuracion::NUMERO_LIMITE_ITERACIONES>0 &&
    Configuracion::OLVIDO_LOGARITMICO;
    std::clock_t start;
    double duracion;
    while(iteracion <= numeroIteraciones*Configuracion::NUMERO_DATOS)
    {
        if(!pausarEntrenamiento)
        {
            listoGuardar = false;
            start = std::clock();
            for(int fila = 0; fila < Configuracion::NUMERO_DATOS; fila++)
            {
                Arreglos::getFila(entrada, datosEntrenamiento, fila);
                if(Configuracion::NUMERO_HILOS>1){
                    inicio_calculo_hilos = true;
                    bool termino_calculo=false;
                    while(termino_calculo==false){
                        termino_calculo = true;
                        for(int i=0; i<Configuracion::NUMERO_HILOS; i++){
                            if(hilodeSeleccion[i].estado_calculo_finalizado==false){
                                termino_calculo =false;
                            }
                        }
                    }
                    inicio_calculo_hilos = false;
                    double distancia_menor = std::numeric_limits<double>::infinity();
                    int indice_ganador =-1;
                    for(int i=0; i<Configuracion::NUMERO_HILOS; i++){
                        if(hilodeSeleccion[i].valor_neurona_ganadora<distancia_menor){
                            indice_ganador=hilodeSeleccion[i].indice_neurona_ganadora;
                            distancia_menor=hilodeSeleccion[i].valor_neurona_ganadora;
                        }
                        /*printf("inf[%d]=sup[%d]=indice[%d] = valor[%f]\n",hilodeSeleccion[i].limite_inferior,
                               hilodeSeleccion[i].limite_superior, hilodeSeleccion[i].indice_neurona_ganadora,
                               hilodeSeleccion[i].valor_neurona_ganadora);*/

                        hilodeSeleccion[i].estado_calculo_finalizado=false;
                    }
                    indiceNeuronaGanadora = indice_ganador;
                }else{
                    indiceNeuronaGanadora = seleccionNeuronaGanadora();
                }
                printf("indice ganador = %d\n", indiceNeuronaGanadora);

                aprendizaje(indiceNeuronaGanadora);
                /*
                if(entrada[Configuracion::NUMERO_ENTRADAS-1]*800<150)
                {
                    if(Arreglos::fRand(0, 1) > 0.7)
                    {
                        indiceNeuronaGanadora = seleccionNeuronaGanadora();
                        aprendizaje(indiceNeuronaGanadora);
                    }

                }
                else
                {
                    indiceNeuronaGanadora = seleccionNeuronaGanadora();
                        aprendizaje(indiceNeuronaGanadora);
                }
                */
                if(flag_olvido_progresivo){
                    for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
                    {
                        olvidoLogaritmico(&alfas[i], alfa, (fila/Configuracion::NUMERO_DATOS)*
                                          Configuracion::NUMERO_LIMITE_ITERACIONES);
                    }
                }
                iteracion+=1;
            }
            duracion = ( std::clock() - start );
            printf("tiempo seleccion neurona: %f\n", duracion);
            if(!flag_olvido_progresivo){
                for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
                {
                    olvidoProgresivo(&alfas[i], beta);
                }
            }
            if(ciclos>Configuracion::NUMERO_LIMITE_ITERACIONES &&
               Configuracion::NUMERO_LIMITE_ITERACIONES>0){
               listoGuardar = true;
                break;
           }


            ciclos +=1;
            listoGuardar = true;
        }

    }
    printf("termino de entrenar????\n");
    terminoEntrenarse = true;
}

double** SOM::getRedNeuronal()
{
    return redNeuronal;
}


NeuronaHex** SOM::getMapaHex()
{
    return mapaHex;
}

void SOM::setRedNeuronal(double **red)
{
    redNeuronal = red;
}

void SOM::setPausar(bool pause)
{
    pausarEntrenamiento = pause;
}

bool SOM::getListoGuardar()
{
    return listoGuardar;
}

double SOM::getAlfa()
{
    return alfa;
}
double SOM::getBeta()
{
    return beta;
}
double SOM::setAlfa(double alfa1)
{
    alfa = alfa1;
}
double SOM::setBeta(double beta1)
{
    beta = beta1;
}

double *SOM::getAlfas()
{
    return alfas;
}
double *SOM::getBetas()
{
    return betas;
}

double SOM::setAlfas(double *alfa1)
{
    alfas=alfa1;
}

double SOM::setBetas(double *beta1)
{
    betas = beta1;
}

bool SOM::getTerminoEntrenarse()
{
    return terminoEntrenarse;
}

void SOM::setTerminoEntrenarse(bool valor)
{
    terminoEntrenarse = valor;
}

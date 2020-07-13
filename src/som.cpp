#include "../include/som.h"
#include <stdio.h>
#include <sstream>

void *funcion_hilo_seleccion(void *funcion)
{
    parametrosHIlo param = *(parametrosHIlo*)funcion;
    int indice = param.indice_hilo;
    SOM *som1 = param.som1;
    int indice_neurona;
    std::string cadena;
    par_res par;
    par.indice_hilo = indice;

    std::stringstream ss;
    ss<<indice;
    int contador = 1;
    double sumatoria_distancia = 0;

    std::ofstream respaldo_hilo;
    respaldo_hilo.open ("hilo_respaldo_distancias"+ss.str()+".txt");
    while(!som1->terminoEntrenarse){
        if(som1->inicio_calculo_hilos && !som1->hilodeSeleccion[indice].estado_calculo_finalizado
        && som1->hilodeSeleccion[indice].estado_calculo_iniciado){
            //som1->respaldo<< "proceso hilo: "<<indice<<"\n";
            double distancia = 0;
            double distanciaAux = std::numeric_limits<double>::infinity();
            int indiceNeuronaGanadora = 0;
            double sum_sub_dis  =0;
            dato_neu dn;
            par.registro.clear();
            for(int indiceNeu=som1->hilodeSeleccion[indice].limite_inferior; indiceNeu<som1->hilodeSeleccion[indice].limite_superior; indiceNeu++) {
                Arreglos::getNeurona(som1->hilodeSeleccion[indice].neurona, som1->redNeuronal, indiceNeu);
                distancia = Distancias::distanciaEuclidea_1(som1->entrada,
                                                            som1->hilodeSeleccion[indice].neurona);//distanciaEuclidea_1(entrada, neurona);
                som1->sumatoria_distancias+=distancia;
                sumatoria_distancia+=distancia;
                sum_sub_dis+=distancia;
                for (int i = 0; i <Configuracion::NUMERO_ENTRADAS ; ++i) {
                    dn.neurona[i]=som1->hilodeSeleccion[indice].neurona[i];
                    //respaldo_hilo << "[" << som1->hilodeSeleccion[indice].neurona[i]<<"]";
                }
                //respaldo_hilo <<"\n";
                dn.valor_distancia=distancia;
                dn.indice_neurona=indiceNeu;
                par.registro.push_back(dn);
                if (distancia < distanciaAux) {
                    distanciaAux = distancia;
                    indiceNeuronaGanadora = indiceNeu;
                }
            }
            respaldo_hilo << "hilo etapa: " << som1->etapa_pos_seleccion<<"\n";
            respaldo_hilo << "hilo_indice hilo: " << indice<<"\n";
            respaldo_hilo << "hilo_indice fila: " << som1->fila_dato<<"\n";
            respaldo_hilo << "hilo_valor distancia: " << distanciaAux<<"\n";
            respaldo_hilo << "hilo_indice neurona: " << indiceNeuronaGanadora<<"\n";
            respaldo_hilo << "hilo_sum_dis: " << sum_sub_dis<<"\n";
            respaldo_hilo << "contador: " << contador<<"\n";
            double e;
            for (int j = 0; j < Configuracion::NUMERO_ENTRADAS; ++j) {
                e=som1->entrada[j];
                respaldo_hilo << "[" << e <<"]";
            }
            respaldo_hilo <<"\n";
            respaldo_hilo <<"-------------------------\n";
            contador+=1;

            par.indice_neurona=indiceNeuronaGanadora;
            par.valor_distancia=distanciaAux;
            par.fila_datos=som1->fila_dato;
            som1->respaldo_td.push_back(par);
            som1->hilodeSeleccion[indice].valor_neurona_ganadora=distanciaAux;
            som1->hilodeSeleccion[indice].indice_neurona_ganadora=indiceNeuronaGanadora;
            som1->hilodeSeleccion[indice].estado_calculo_finalizado=true;
            bool esperar_otros_hilos = true;
            while (esperar_otros_hilos){
                bool termiron_otros_hilos = true;
                for(int i=0; i<Configuracion::NUMERO_HILOS; i++){
                    if(indice != i){
                        if(som1->hilodeSeleccion[i].estado_calculo_finalizado == false){
                            termiron_otros_hilos = false;
                        }
                    }
                }
                esperar_otros_hilos = !termiron_otros_hilos;
            }
        }
    }
    printf("termino del hilo [%d]\n",indice);
    printf("sumatoria distancia [%d]: %f\n", indice, sumatoria_distancia);
    respaldo_hilo.close();
    //pthread_exit(NULL);
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

    for(int i=0; i<hilodeSeleccion.size(); i++)
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
            redNeuronal[j][i]=0.2;//Arreglos::fRand(0, 1);
        }
    }
}

int SOM::seleccionNeuronaGanadora()
{
    par_res par;
    par.indice_hilo = -1;

    double distancia = 0;
    double distanciaAux = std::numeric_limits<double>::infinity();
    int indiceNeuronaGanadora = 0;
    dato_neu dn;
    for(int indiceNeu=0; indiceNeu<numeroNeuronas; indiceNeu++)
    {
        Arreglos::getNeurona(neurona, redNeuronal, indiceNeu);
        distancia = Distancias::distanciaEuclidea_1(entrada, neurona);//distanciaEuclidea_1(entrada, neurona);
        sumatoria_distancias+=distancia;
        for (int i = 0; i <Configuracion::NUMERO_ENTRADAS ; ++i) {
            dn.neurona[i]=neurona[i];
        }
        dn.valor_distancia=distancia;
        dn.indice_neurona=indiceNeu;
        par.registro.push_back(dn);
        if(distancia < distanciaAux)
        {
            distanciaAux = distancia;
            indiceNeuronaGanadora = indiceNeu;
        }
    }


    par.valor_distancia=distanciaAux;
    par.indice_neurona=indiceNeuronaGanadora;
    par.fila_datos=this->fila_dato;
    respaldo_td.push_back(par);

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

    if (remove("respaldo_distancias.txt") !=0){
        printf("removido el archivo respaldo_distancias.txt\n");
    }
    else{
        printf("la operacion de remover el archivo a fallado\n");
    }

    respaldo.open ("respaldo_distancias.txt");
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
                this->fila_dato = fila;
                Arreglos::getFila(entrada, datosEntrenamiento, fila);
                if(Configuracion::NUMERO_HILOS>1){
                    etapa_pos_seleccion = "etapa antes de la seleccion neurona";


                    for(int i=0; i<Configuracion::NUMERO_HILOS; i++){
                        hilodeSeleccion[i].estado_calculo_finalizado=false;
                        hilodeSeleccion[i].estado_calculo_iniciado=true;
                    }
                    /*bool termino_calculo=false;
                    while(termino_calculo==false){
                        termino_calculo = true;
                        for(int i=0; i<Configuracion::NUMERO_HILOS; i++){
                            if(hilodeSeleccion[i].estado_calculo_finalizado==false){
                                termino_calculo =false;
                            } else{
                                hilodeSeleccion[i].estado_calculo_iniciado=false;
                            }
                        }
                    }*/
                    inicio_calculo_hilos = true;
                    bool esperar_otros_hilos = true;
                    while (esperar_otros_hilos){
                        bool termiron_otros_hilos = true;
                        for(int i=0; i<Configuracion::NUMERO_HILOS; i++){
                            if(hilodeSeleccion[i].estado_calculo_finalizado == false){
                                termiron_otros_hilos = false;
                            }
                        }
                        esperar_otros_hilos = !termiron_otros_hilos;
                    }

                    inicio_calculo_hilos = false;
                    etapa_pos_seleccion = "etapa depues de la seleccion neurona :D";
                    double distancia_menor = std::numeric_limits<double>::infinity();
                    int indice_ganador =-1;
                    for(int i_=0; i_<Configuracion::NUMERO_HILOS; i_++){
                        if(hilodeSeleccion[i_].valor_neurona_ganadora<distancia_menor){
                            indice_ganador=hilodeSeleccion[i_].indice_neurona_ganadora;
                            distancia_menor=hilodeSeleccion[i_].valor_neurona_ganadora;
                        }
                        /*printf("inf[%d]=sup[%d]=indice[%d] = valor[%f]\n",hilodeSeleccion[i].limite_inferior,
                               hilodeSeleccion[i].limite_superior, hilodeSeleccion[i].indice_neurona_ganadora,
                               hilodeSeleccion[i].valor_neurona_ganadora);*/
                    }
                    etapa_pos_seleccion = "despues de evaluar al ganador :/";
                    indiceNeuronaGanadora = indice_ganador;
                }else{
                    indiceNeuronaGanadora = seleccionNeuronaGanadora();
                }
                printf("indice ganador = %d\n", indiceNeuronaGanadora);

                //aprendizaje(indiceNeuronaGanadora);

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
            printf("distancia acumulado: %f\n", this->sumatoria_distancias);
            if(!flag_olvido_progresivo){
                for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
                {
                    olvidoProgresivo(&alfas[i], beta);
                }
            }

            ciclos +=1;

            if(ciclos>=Configuracion::NUMERO_LIMITE_ITERACIONES &&
               Configuracion::NUMERO_LIMITE_ITERACIONES>0){
               listoGuardar = true;
                break;
           }

            listoGuardar = true;
        }

    }

    for (int indice = 0; indice < respaldo_td.size(); ++indice) {
        respaldo << "indice hilo: " << respaldo_td[indice].indice_hilo<<"\n";
        respaldo << "indice fila: " << respaldo_td[indice].fila_datos<<"\n";
        respaldo << "valor distancia: " << respaldo_td[indice].valor_distancia<<"\n";
        respaldo << "indice neurona: " << respaldo_td[indice].indice_neurona<<"\n";
        for (int indice_naurona = 0; indice_naurona < respaldo_td[indice].registro.size(); ++indice_naurona) {
            dato_neu dn = respaldo_td[indice].registro[indice_naurona];
            respaldo << "----- indice_neurona: " <<dn.indice_neurona << " valor_distancia: "<< dn.valor_distancia<<" neurona[";
            for (int i = 0; i <Configuracion::NUMERO_ENTRADAS ; ++i) {
                respaldo << dn.neurona[i] <<", ";
            }
            respaldo <<"]\n";
        }
    }
    printf("termino de entrenar: %f\n", this->sumatoria_distancias);
    respaldo.close();
    terminoEntrenarse = true;
    if(Configuracion::NUMERO_HILOS>1){
        for(int i=0; i<Configuracion::NUMERO_HILOS; i++){
            pthread_join(hilodeSeleccion[i].hilo, NULL);
        }
    }
    pthread_exit(NULL);

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

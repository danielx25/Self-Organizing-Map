#include "som.h"
#include <stdio.h>



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

}

SOM::~SOM()
{
    //dtor
    /**Liberando memoria de la matriz marcasMapa*/
    for(int i=0; i<Configuracion::ANCHO; i++)
        delete[] marcasMapa[i];
    delete []marcasMapa;
}

/**
funciones de medicion de Diatancias entre dos puntos de n dimensiones
*/
double SOM::distanciaEuclidea(double *entrada, double *pesos)
{
    double sumatoria = 0;

    for(int i=0; i < numeroEntradas; i++)
    {
        sumatoria+= pow(entrada[i]- pesos[i], 2);
    }
    return sqrt(sumatoria);
}


double SOM::distanciaManhattan(double *entrada, double *pesos)
{
    double sumatoria = 0;

    for(int i=0; i < numeroEntradas; i++)
    {
        sumatoria+= abs(entrada[i]- pesos[i]);
    }
    return sumatoria;
}

double SOM::distanciaMinkowski(double *entrada, double *pesos, double p)
{
    double sumatoria = 0;

    for(int i=0; i < numeroEntradas; i++)
    {
        sumatoria+= pow(entrada[i]- pesos[i], p);
    }
    return pow(sumatoria, 1/p);
}

/**establece un nivel de importancia mayor en algunas de la dimensiones*/
double SOM::distanciaEuclidea_1(double *entrada, double *pesos)
{
    double sumatoria = 0;

    for(int i=0; i < numeroEntradas; i++)
    {
        if(i!=36)
            sumatoria+= 0.021621621621621623*pow(entrada[i]- pesos[i], 2);
        else{
            sumatoria+= 0.22162162162162163*pow(entrada[i]- pesos[i], 2);
        }
    }
    return sqrt(sumatoria);
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

void SOM::olvidoLogaritmico(double *alfa, double alfa0,int iteracion, int totalIter)
{
    *alfa = alfa0*exp(-4*iteracion/totalIter);

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

    for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
    {
        redNeuronal[i][indiceNeurona]+=aprendizajeHebb(alfas[i], 1, entrada[i], redNeuronal[i][indiceNeurona]);
    }
    propagacionAprendizaje(2, fila, columna);
}

void SOM::pesosAleatorios()
{
    for(int i=0; i<numeroNeuronas; i++)
    {
        for(int j=0; j<numeroEntradas; j++)
        {
            redNeuronal[j][i]=Arreglos::fRand(0, 1);
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
        //distancia = distanciaEuclidea(entrada, neurona);
        //distancia = distanciaManhattan(entrada, neurona);
        distancia = distanciaEuclidea_1(entrada, neurona);

        if(distancia < distanciaAux)
        {
            distanciaAux = distancia;
            indiceNeuronaGanadora = indiceNeu;
        }
    }
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
    printf("numero iter: %d\n", numeroIteraciones);

    while(iteracion < numeroIteraciones*Configuracion::NUMERO_DATOS)
    {
        if(!pausarEntrenamiento)
        {
            listoGuardar = false;
            for(int fila = 0; fila < Configuracion::NUMERO_DATOS; fila++)
            {
                Arreglos::getFila(entrada, datosEntrenamiento, fila);
                indiceNeuronaGanadora = seleccionNeuronaGanadora();
                aprendizaje(indiceNeuronaGanadora);
                iteracion+=1;
            }
            for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
            {
                olvidoProgresivo(&alfas[i], beta);
                if(i == Configuracion::NUMERO_ENTRADAS-1)
                    olvidoLogaritmico(&alfas[i], alfa, ciclos, numeroIteraciones);
            }
            ciclos +=1;
            listoGuardar = true;
        }

    }
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

#include "Validacion.h"

Validacion::Validacion()
{
    //ctor
    entrada = new double[Configuracion::NUMERO_ENTRADAS];
    neurona = new double[Configuracion::NUMERO_ENTRADAS];
    numeroAciertos = 0;
    acertividad = 0;
    radioMp10 = 0.0125;//0.0625;//0.125;
}

Validacion::~Validacion()
{
    //dtor
}

/**establece un nivel de importancia mayor en algunas de la dimensiones*/
double Validacion::distanciaEuclidea(double *entrada, double *pesos)
{
    double sumatoria = 0;

    for(int i=0; i < Configuracion::NUMERO_ENTRADAS; i++)
    {
        if(i!=Configuracion::NUMERO_ENTRADAS-1)
            sumatoria+= 0.025*pow(entrada[i]- pesos[i], 2);
        else{
            sumatoria+= 0.23125*pow(entrada[i]- pesos[i], 2);
        }
    }
    return sqrt(sumatoria);
}

/*

double Validacion::distanciaEuclidea(double *entrada, double *pesos)
{
    double sumatoria = 0;

    for(int i=0; i < Configuracion::NUMERO_ENTRADAS-1; i++)
    {
        sumatoria+= pow(entrada[i]- pesos[i], 2);
    }
    return sqrt(sumatoria);
}
*/

int Validacion::seleccionNeuronaGanadora()
{
    double distancia = 0;
    double distanciaAux = std::numeric_limits<double>::infinity();
    int indiceNeuronaGanadora = 0;
    for(int indiceNeu=0; indiceNeu<numeroNeuronas; indiceNeu++)
    {
        Arreglos::getNeurona(neurona, pesosRNA, indiceNeu);
        distancia = distanciaEuclidea(entrada, neurona);

        if(distancia < distanciaAux)
        {
            distanciaAux = distancia;
            indiceNeuronaGanadora = indiceNeu;
        }
    }
    return indiceNeuronaGanadora;
}

void Validacion::setDatosEntrenamiento(double **datos)
{
    datosEntrenamiento = datos;
}
void Validacion::setPesosRNA(double **datos, int numero)
{
    pesosRNA = datos;
    numeroNeuronas = numero;

}
void Validacion::iniciarValidacion()
{
    int ganador = 0;
    double mp10Real = 0;
    double mp10Predecion = 0;
    double mp10Inferior = 0;
    double mp10Superior = 0;
    numeroAciertos = 0;

    int *arregloNeurona = new int[Configuracion::NUMERO_NEURONAS];
    for(int i=0; i<Configuracion::NUMERO_NEURONAS; i++)
        arregloNeurona[i]=0;

    int numeroAlertasM = 0;
    int numeroAciertosM = 0;
    for(int indice = 0; indice < Configuracion::NUMERO_DATOS; indice++)
    {
        Arreglos::getFila(entrada, datosEntrenamiento, indice);
        ganador=seleccionNeuronaGanadora();
        Arreglos::getNeurona(neurona, pesosRNA, ganador);
        mp10Predecion = neurona[Configuracion::NUMERO_ENTRADAS-1];
        mp10Real = entrada[Configuracion::NUMERO_ENTRADAS-1];
        mp10Inferior = mp10Predecion - radioMp10;
        mp10Superior = mp10Predecion + radioMp10;
        arregloNeurona[ganador]+=1;

        //printf(" %f < %f &&  %f >= %f\n", mp10Inferior, mp10Real, mp10Superior, mp10Real);
        if(mp10Inferior<mp10Real && mp10Superior>mp10Real)
        {
            numeroAciertos+=1;
            //printf("predecion: %f ---- real: %f \n", mp10Predecion, mp10Real);
        }

        if(mp10Real>0.1875)
        {
            numeroAlertasM +=1;
            if(mp10Inferior<mp10Real && mp10Superior>mp10Real)
                numeroAciertosM+=1;
        }

        //printf("predecion: %f ---- real: %f \n", mp10Predecion, mp10Real);
    }
    printf("numero aciertos: %d\n", numeroAciertos);
    printf("numero alertas mayores   : %d\n", numeroAlertasM);
    printf("numero aciertos alertas M: %d\n", numeroAciertosM);

    int numeroNeuronasActivas = 0;
    for(int i=0; i<Configuracion::NUMERO_NEURONAS; i++)
    {
        if (arregloNeurona[i] > 0 )
            numeroNeuronasActivas+=1;
    }
    printf("numero neuronas activas: %d\n", numeroNeuronasActivas);
}

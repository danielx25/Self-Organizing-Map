#include <iostream>
#include <stdio.h>
#include <limits>
#include "include/som.h"
#include "include/neurona.h"
#include "include/ficherorna.h"
#include "include/configuracion.h"
#include <stdlib.h>     /* atof */
#include <time.h>
#include <pthread.h>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include "include/gestionadorsom.h"
#include "include/Validacion.h"
using namespace std;

void *proceso_hilos(void *funcion)
{
    SOM *som1 = (SOM*)funcion;
    som1->entrenamiento();
    pthread_exit(NULL);
}


static void validar_red()
{
    if(!FicheroRNA::leerConfiguracion())
        FicheroRNA::crearConfiguracion();

    double ** pesosRNA;
    double ** datosEntrenamiento;
	//datosEntrenamiento = createByteMatrix(Configuracion::NUMERO_DATOS, Configuracion::NUMERO_ENTRADAS);
    //pesosRNA = createByteMatrix(Configuracion::NUMERO_NEURONAS, Configuracion::NUMERO_ENTRADAS);

    pesosRNA=new double*[Configuracion::NUMERO_ENTRADAS];
    for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
        pesosRNA[i] = new double[Configuracion::NUMERO_NEURONAS];

    Validacion *validacion = new Validacion();

    if(FicheroRNA::leerCSV(Configuracion::RUTA_ARCHIVO, datosEntrenamiento))
    {

        FicheroRNA::leerPesosRNA(pesosRNA);
        validacion->setDatosEntrenamiento(datosEntrenamiento);
        validacion->setPesosRNA(pesosRNA, Configuracion::NUMERO_NEURONAS);
        validacion->iniciarValidacion();

    }

}

static void proceso_principal()
{
    //iniciando la configuracion desde el archivo
    if(!FicheroRNA::leerConfiguracion())
        FicheroRNA::crearConfiguracion();

    pthread_t hilo;
    double ** BitmapArray = NULL;
	BitmapArray = Arreglos::creandoMatrizDouble(Configuracion::NUMERO_DATOS, Configuracion::NUMERO_ENTRADAS);
    if(FicheroRNA::leerCSV(Configuracion::RUTA_ARCHIVO, BitmapArray))
    {
        SOM *somAux = new SOM(BitmapArray);

        bool leerStatus = FicheroRNA::leerStatusRNA(somAux);
        if(!leerStatus)
            printf("No se pudo leer Status de la RNA :( \n");

        bool leerPesosRNA = FicheroRNA::leerPesosRNA(somAux->getRedNeuronal());
        if(!leerPesosRNA)
            printf("No se pudo leer los pesos RNA :( \n");


        SOM *som1;//= new SOM(BitmapArray);

        if(leerPesosRNA  && leerStatus)
        {
            int opcion = 1;
            printf("Entrenar de inicio.\n");
            printf("1.- SI.\n");
            printf("2.- NO.\n");
            //scanf("%d", &opcion);
            if(opcion == 1)
                som1 = new SOM(BitmapArray);
            else
                som1 = somAux;
        }
        else
        {
            som1 = new SOM(BitmapArray);
            delete somAux;
        }

        if(!som1->getTerminoEntrenarse())
        {
            //pthread_create(&hilo, NULL, proceso_hilos, (void*)som1);
            som1->entrenamiento();
            double porcentaje = 0;
            //system("cls");

            //for(int i=0; i<som1.numeroIteraciones*Configuracion::NUMERO_DATOS+10;i++)
            /*do
            {
                porcentaje = (som1->iteracion)/(double)(som1->numeroIteraciones*Configuracion::NUMERO_DATOS);
                //printProgress(porcentaje, som1->numeroIteraciones*Configuracion::NUMERO_DATOS, som1->iteracion);
                Sleep(10000);
                som1->setPausar(true);
                while(som1->getListoGuardar() == false)
                {
                }
                if(som1->getListoGuardar())
                {
                    FicheroRNA::escribirJS(Configuracion::ANCHO, Configuracion::LARGO, som1->getMapaHex(), som1->getRedNeuronal());
                    FicheroRNA::guardarPesosRNA(som1->getRedNeuronal());
                    FicheroRNA::guardarStatusRNA(som1);
                }
                som1->setPausar(false);
            }while(som1->iteracion<=som1->numeroIteraciones*Configuracion::NUMERO_DATOS && !som1->terminoEntrenarse);*/
            printf("Termino de entrenarse!!!\n");
        }
        else
            printf("Termino de entrenarse!!!\n");


    }

}


int main()
{
    int opcion = 1;

    printf("1.- Entrenar.\n");
    printf("2.- Validar.\n");
    //scanf("%d", &opcion);
    if(opcion == 1){
        proceso_principal();
    }
    else{
        validar_red();
    }


    //system("pause");
    //return 0;
}


static void ejemplo()
{
    double **matriz = new double*[30];

    for(int i = 0; i < 30; i++)
    {
        matriz[i] = new double[4];
    }

    double ejemplo1[30][4] = {
        {0.25, 0.375, 0.428571428571, 0.25},
        {0.583333333333, 0.625, 0.428571428571, 0.598484848485},
        {0.333333333333, 1.0, 0.571428571429, 0.719696969697},
        {0.75, 0.625, 0.428571428571, 0.704545454545},
        {0.5, 0.0, 0.428571428571, 0.204545454545},
        {0.333333333333, 1.0, 0.428571428571, 0.643939393939},
        {0.25, 0.125, 0.857142857143, 0.340909090909},
        {0.75, 0.375, 0.0, 0.340909090909},
        {0.25, 0.75, 0.714285714286, 0.606060606061},
        {0.666666666667, 0.125, 0.142857142857, 0.227272727273},
        {0.583333333333, 0.25, 0.428571428571, 0.393939393939},
        {0.166666666667, 0.5, 0.714285714286, 0.416666666667},
        {0.166666666667, 0.5, 0.714285714286, 0.416666666667},
        {0.583333333333, 0.375, 0.857142857143, 0.689393939394},
        {0.5, 0.625, 0.142857142857, 0.393939393939},
        {0.0, 0.875, 0.0, 0.136363636364},
        {1.0, 0.375, 0.285714285714, 0.651515151515},
        {0.416666666667, 0.0, 0.142857142857, 0.0},
        {0.0, 0.625, 0.285714285714, 0.151515151515},
        {0.166666666667, 0.5, 1.0, 0.568181818182},
        {0.75, 0.125, 0.142857142857, 0.280303030303},
        {0.666666666667, 0.875, 0.571428571429, 0.863636363636},
        {0.166666666667, 0.125, 0.571428571429, 0.136363636364},
        {0.0833333333333, 0.25, 1.0, 0.378787878788},
        {0.416666666667, 1.0, 1.0, 1.0},
        {0.333333333333, 0.625, 0.142857142857, 0.287878787879},
        {0.0, 0.375, 0.428571428571, 0.0909090909091},
        {0.5, 0.5, 0.142857142857, 0.325757575758},
        {1.0, 0.375, 0.714285714286, 0.878787878788},
        {0.0833333333333, 0.375, 1.0, 0.44696969697}
    };

    for(int i = 0; i<30; i++)
    {
        for(int j=0; j<4; j++)
        {
            matriz[i][j] = ejemplo1[i][j];
        }
    }

    GestionadorSOM *gestion_RNA = new GestionadorSOM();

    gestion_RNA->inicializarSOM(matriz, 30, 4);
    gestion_RNA->empezarEntrenamiento();

    /*

    }
    */
    //std::cin.get();

}

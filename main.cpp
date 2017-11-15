#include <iostream>
#include <stdio.h>
#include <limits>
#include "som.h"
#include "neurona.h"
#include "ficherorna.h"
#include "configuracion.h"
#include <stdlib.h>     /* atof */
#include <time.h>
#include <pthread.h>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <thread>         // std::thread
#include "gestionadorsom.h"
using namespace std;

/*

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress (double percentage, int totalIter, int iter)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("total: %d | iteracion: %d | por: %f", totalIter, iter, percentage);
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}
*/
void *proceso_hilos(void *funcion)
{
    SOM *som1 = (SOM*)funcion;
    som1->entrenamiento();
    pthread_exit(NULL);
}


double ** createByteMatrix(unsigned int rows, unsigned int cols)
{
    // declaration
	double ** a;
	// allocation
	a = new double*[rows];
	for(int i = 0; i < rows; i++)
		a[i] = new double[cols];
    return a;
}

static void proceso_principal()
{
    //iniciando la configuracion desde el archivo
    if(!FicheroRNA::leerConfiguracion())
        FicheroRNA::crearConfiguracion();

    pthread_t hilo;
    double ** BitmapArray;
	BitmapArray = createByteMatrix(Configuracion::NUMERO_DATOS, Configuracion::NUMERO_ENTRADAS);

    if(FicheroRNA::leerCSV(Configuracion::RUTA_ARCHIVO, BitmapArray))
    {
        printf("iniciando entrenamiento\n");
        SOM som1(BitmapArray);

        if(FicheroRNA::leerStatusRNA(&som1))
        {
            FicheroRNA::leerPesosRNA(som1.getRedNeuronal());
        }

        pthread_create(&hilo, NULL, proceso_hilos, (void*)&som1);

        double porcentaje = 0;
        system("cls");
        for(int i=0; i<som1.numeroIteraciones*Configuracion::NUMERO_DATOS+10;i++)
        {
            porcentaje = (som1.iteracion)/(double)(som1.numeroIteraciones*Configuracion::NUMERO_DATOS);
            printProgress(porcentaje, som1.numeroIteraciones*Configuracion::NUMERO_DATOS, som1.iteracion);
            Sleep(2000);
            som1.setPausar(true);
            while(som1.getListoGuardar() == false)
            {
            }
            if(som1.getListoGuardar())
            {
                FicheroRNA::escribirJS(Configuracion::ANCHO, Configuracion::LARGO, som1.getMapaHex(), som1.getRedNeuronal());
                FicheroRNA::guardarPesosRNA(som1.getRedNeuronal());
                FicheroRNA::guardarPesosRNA_LVQ(som1.getRedNeuronal(), som1.getMapaHex());
                FicheroRNA::guardarStatusRNA(&som1);
            }
            som1.setPausar(false);
        }
    }
    else{
        printf("fichero no encontrado\n");
    }
}

int main()
{

    proceso_principal();

    return 0;
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

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
using namespace std;


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

int main()
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

        //som1.entrenamiento();


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
                printf("esperando para guardar\n");
            }
            if(som1.getListoGuardar())
            {
                printf("guardar\n");
                FicheroRNA::escribirJS(Configuracion::ANCHO, Configuracion::LARGO, som1.getMapaHex(), som1.getRedNeuronal());
                FicheroRNA::guardarPesosRNA(som1.getRedNeuronal());
                FicheroRNA::guardarStatusRNA(&som1);
            }
            som1.setPausar(false);

        }


    }
    else{
        printf("fichero no encontrado\n");
    }
    std::cin.get();
    //FicheroRNA::crearConfiguracion();



    return 0;
}

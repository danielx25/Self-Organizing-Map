#include <iostream>
#include <stdio.h>
#include <limits>
#include "som.h"
#include "neurona.h"
#include "ficherorna.h"
#include "configuracion.h"
#include <stdlib.h>     /* atof */
#include <time.h>


using namespace std;




double ** createByteMatrix(unsigned int rows, unsigned int cols) {
/*
	double ** matrix;
	matrix = (double **) calloc(cols, sizeof(double *));
	for(unsigned int i = 0; i < cols; i++)
		matrix[i] = (double *) calloc(rows, sizeof(double));
	return matrix;
*/
    // declaration
	double ** a;
    printf("fila: %d\n", rows);
    printf("columna: %d\n", cols);
	// allocation
	a = new double*[rows];
	for(int i = 0; i < rows; i++)
		a[i] = new double[cols];
    return a;
}

int main()
{
    //double ejemplo[Configuracion::NUMERO_DATOS][Configuracion::NUMERO_ENTRADAS];
    clock_t start, end;
    start = clock();


    //double arreglo[Configuracion::NUMERO_DATOS][Configuracion::NUMERO_ENTRADAS];
    //double arreglo1[65000][90000];

    double ** BitmapArray;
	BitmapArray = createByteMatrix(Configuracion::NUMERO_DATOS, Configuracion::NUMERO_ENTRADAS);

    //NeuronaHex mapaHex[Configuracion::ANCHO][Configuracion::LARGO];
    //crearMatrizConexionHex(mapaHex);
    //cout << "Hello world!" << endl;
    FicheroRNA::leerCSV("DatosEntrenamiento.csv", BitmapArray);
    double a[2]={1,1};
    double b[2]={2,2};
    SOM som1(BitmapArray);
    som1.entrenamiento();
    //som1.ejemplo1();
    //FicheroRNA::escribirJS(Configuracion::ANCHO, Configuracion::LARGO, mapaHex);



    /* fin de las instrucciones*/

    end = clock();
    printf("The time was: %f\n", (end - start) / CLK_TCK);

    return 0;
}

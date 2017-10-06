#include <iostream>
#include <stdio.h>
#include <limits>
#include "som.h"
#include "neurona.h"
#include "ficherorna.h"
#include "configuracion.h"
#include <stdlib.h>     /* atof */

using namespace std;




int main()
{
    double ejemplo[5][3];

    //NeuronaHex mapaHex[Configuracion::ANCHO][Configuracion::LARGO];
    //crearMatrizConexionHex(mapaHex);
    //cout << "Hello world!" << endl;
    printf("aver\n");
    FicheroRNA::leerCSV("Libro1.csv", ejemplo);
    //SOM som1(ejemplo);
    //som1.entrenamiento();
    //som1.ejemplo1();
    //FicheroRNA::escribirJS(Configuracion::ANCHO, Configuracion::LARGO, mapaHex);




    return 0;
}

#include "configuracion.h"
#include <iostream>
Configuracion::Configuracion()
{
    //ctor

}

Configuracion::~Configuracion()
{
    //dtor
}

std::string Configuracion::RUTA_ARCHIVO = "DatosEntrenamientov3.csv";
unsigned int Configuracion::NUMERO_ENTRADAS=32;
int Configuracion::NUMERO_NEURONAS = 600;
int Configuracion::RANGO_VECINDAD = 4;
int Configuracion::NUMERO_DATOS = 62385;
/**el largo puede ser cualquier pero el ancho tiene que ser par(para que la estructura hexagonal pueda unirse en sus limites)
como un balon de futbol con caras hexagonales*/
int Configuracion::ANCHO = 24;
int Configuracion::LARGO = 25;
//tasa de aprendizaje
double Configuracion::ALFA = 0.5;
//decrecimiento de la tasa de aprendizaje
double Configuracion::BETA = 0.005;

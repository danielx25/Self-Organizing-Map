#include "configuracion.h"

Configuracion::Configuracion()
{
    //ctor

}

Configuracion::~Configuracion()
{
    //dtor
}
unsigned int Configuracion::NUMERO_ENTRADAS=38;
int Configuracion::NUMERO_NEURONAS = 1600;
int Configuracion::RANGO_VECINDAD = 4;
int Configuracion::NUMERO_DATOS = 62385;
/**el largo puede ser cualquier pero el ancho tiene que ser par(para que la estructura hexagonal pueda unirse en sus limites)
como un balon de futbol con caras hexagonales*/
int Configuracion::ANCHO = 40;
int Configuracion::LARGO = 40;
//tasa de aprendizaje
double Configuracion::ALFA = 0.5;
//decrecimiento de la tasa de aprendizaje
double Configuracion::BETA = 0.005;

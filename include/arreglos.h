#ifndef ARREGLOS_H
#define ARREGLOS_H

#include <stdlib.h>
#include "configuracion.h"
class Arreglos
{
    public:
        Arreglos() {}
        virtual ~Arreglos() {}

        static int len(double *arreglo)
        {
            return sizeof(arreglo)/sizeof(*arreglo);
        }

        static double fRand(double fMin, double fMax)
        {
            double f = (double)rand() / RAND_MAX;
            return fMin + f * (fMax - fMin);
        }
        //double **redNeuronal;//[Configuracion::NUMERO_ENTRADAS][Configuracion::NUMERO_NEURONAS];
        static void getNeurona(double *arreglo1, double **matriz, int indiceNeurona)
        {
            for(int fila=0; fila<Configuracion::NUMERO_ENTRADAS; fila++)
            {
                arreglo1[fila] =  matriz[fila][indiceNeurona];
            }
        }

        static void getFila(double *arreglo1, double **matriz, int fila)
        {
            for(int columna=0; columna<Configuracion::NUMERO_ENTRADAS; columna++)
            {
                arreglo1[columna] =matriz[fila][columna];
            }
        }

        static double ** creandoMatrizDouble(unsigned int rows, unsigned int cols)
        {
            // declaration
            double ** a;
            // allocation
            a = new double*[rows];
            for(int i = 0; i < rows; i++)
                a[i] = new double[cols];
            return a;
        }


    protected:

    private:
};

#endif // ARREGLOS_H

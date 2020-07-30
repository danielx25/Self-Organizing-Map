#ifndef GESTIONADORSOM_H
#define GESTIONADORSOM_H

#include "som.h"
#include "ficherorna.h"
#include "configuracion.h"

#include <pthread.h>


#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

static void printProgress (double percentage, int totalIter, int iter, int ciclo)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("total: %d | iteracion: %d | por: %f| ciclo: %d", totalIter, iter, percentage,ciclo);
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}

class GestionadorSOM
{
    public:
        GestionadorSOM();
        virtual ~GestionadorSOM();

        void inicializarSOM(double **datosEntrenamiento1, int numeroDatos, int numEntrada);
        void inicializarSOM(double **datosEntrenamiento1, int numeroDatos, int numEntrada, int alfa, int beta, int numeroNeuronas, int ancho, int largo);

        void empezarEntrenamiento();
        void reanudarEntrenamiento();

        void statusProgresoRNA();

        void guardarEstadoRNA();

        double getProgreso();

        void main1();
        //void

    protected:

    private:
        SOM *som1;
        double **datosEntrenamiento;
        double progreso;//porcentaje

        pthread_t hilo;//hilo del entrenamiento de la red
        //milisegundo
        int tiempoGuardadoRed = 2000;

};

#endif // GESTIONADORSOM_H

#ifndef GESTIONADORSOM_H
#define GESTIONADORSOM_H

#include "som.h"
#include "ficherorna.h"
#include "configuracion.h"

class GestionadorSOM
{
    public:
        GestionadorSOM();
        virtual ~GestionadorSOM();

        void inicializarSOM(double **datosEntrenamiento1, int numeroDatos);
        void inicializarSOM(double **datosEntrenamiento1, int numeroDatos, int alfa, int beta, int numeroNeuronas, int ancho, int largo);

        void empezarEntrenamiento();
        void reanudarEntrenamiento();

    protected:

    private:
        SOM *som1;
        double **datosEntrenamiento;


};

#endif // GESTIONADORSOM_H

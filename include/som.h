#ifndef SOM_H
#define SOM_H

#include <stdlib.h>
#include <math.h>
#include <limits>
#include "NeuronaHex.h"
#include "configuracion.h"
#include "arreglos.h"
//#include "ficherorna.h"


class SOM
{
    public:
        SOM(double **datos);
        virtual ~SOM();

        double distanciaEuclidea(double *entrada, double *pesos);
        double distanciaManhattan(double *entrada, double *pesos);
        double distanciaMinkowski(double *entrada, double *pesos, double p);

        double distanciaEuclidea_1(double *entrada, double *pesos);

        void olvidoProgresivo(double *alfa, double beta);
        void olvidoLogaritmico(double *alfa, double alfa0,int iteracion, int totalIter);

        /**actualizando lo peso de la neurona*/
        void actualizarPesosNeurona(int distanciaVecin, int indiceNeurona);

        void pesosAleatorios();
        int seleccionNeuronaGanadora();
        void entrenamiento();


        void vecindarioCuadrado();
        void vecindarioEstrella();

        double aprendizajeHebb(double alfa, double distanciaVecindario, double e, double u);
        void aprendizaje(int indiceNeurona);
        void propagacionAprendizaje(int distanciaVecin, int fila, int columna);

        void ejemplo1();
        double **getRedNeuronal();
        NeuronaHex **getMapaHex();

        void setRedNeuronal(double **red);
        void setPausar(bool pause);
        bool getListoGuardar();

        int numeroIteraciones;
        int iteracion;

    protected:

    private:
        //malla de la red neuronal
        //neurona redNeuronal[Configuracion::NUMERO_NEURONAS];
        double **datosEntrenamiento;
        double *entrada = new double[Configuracion::NUMERO_ENTRADAS];
        double *neurona = new double[Configuracion::NUMERO_ENTRADAS];
        double **redNeuronal;//[Configuracion::NUMERO_ENTRADAS][Configuracion::NUMERO_NEURONAS];
        int numeroNeuronas;
        int numeroEntradas;
        int rangoVecindad;
        int largo;
        int ancho;

        //tasa de aprendizaje
        double alfa = Configuracion::ALFA;

        //decrecimiento de la tasa de aprendizaje
        double beta = Configuracion::BETA;

        double *alfas = new double [Configuracion::NUMERO_ENTRADAS];
        double *betas = new double [Configuracion::NUMERO_ENTRADAS];

        int indiceNeuronaGanadora;

        /**Estructura hexagonal del mapa de kohonen*/
        NeuronaHex **mapaHex;//[Configuracion::ANCHO][Configuracion::LARGO];
        /**marcar los hexagonos de la neuronas que ya aprendieron en una iteracion*/
        bool **marcasMapa;

        /**pause*/
        bool pausarEntrenamiento;
        /**terminar el entrenamiento forsozamente*/
        bool terminar;
        /**señal para que guarde el estado de la red neuronal*/
        bool listoGuardar;

};

#endif // SOM_H

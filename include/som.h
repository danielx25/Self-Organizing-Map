#ifndef SOM_H
#define SOM_H

#include <stdlib.h>
#include <math.h>
#include <limits>
#include <pthread.h>
#include <ctime>
#include <vector>
#include "NeuronaHex.h"
#include "configuracion.h"
#include "arreglos.h"
#include "distancias.h"

class SOM;
void *funcion_hilo_seleccion();

typedef struct
{
    pthread_t hilo;
    bool estado_calculo_finalizado = false;
    int incide_hilo;
    int limite_inferior;
    int limite_superior;
    int indice_neurona_ganadora;
}HiloSeleccion;

typedef struct
{
    SOM *som1 = NULL;
    int indice_hilo=-1;
}parametrosHIlo;

class SOM
{
    public:
        SOM(double **datos);
        virtual ~SOM();

        void olvidoProgresivo(double *alfa, double beta);
        void olvidoLogaritmico(double *alfa, double alfa0, double ciclo);

        /**actualizando lo peso de la neurona*/
        void actualizarPesosNeurona(int distanciaVecin, int indiceNeurona);

        void pesosAleatorios();
        int seleccionNeuronaGanadora();
        int seleccionNeuronaGanadoraHilo(int indiceHilo);
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

        double getAlfa();
        double getBeta();

        double setAlfa(double alfa1);
        double setBeta(double beta1);

        double *getAlfas();
        double *getBetas();

        double setAlfas(double *alfa1);
        double setBetas(double *beta1);

        bool getTerminoEntrenarse();
        void setTerminoEntrenarse(bool valor);


        int numeroIteraciones;
        /** iteracion por cada ejemplo entrenado por la red*/
        int iteracion;
        /** ciclos de entrenamiento del dataset*/
        int ciclos;

        /**terminar el entrenamiento forsozamente*/
        bool terminoEntrenarse;
        std::vector<HiloSeleccion> hilodeSeleccion;
        bool inicio_calculo_hilos = false;
        bool termino_calculos_hilos = false;
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

        /**señal para que guarde el estado de la red neuronal*/
        bool listoGuardar;

};

#endif // SOM_H

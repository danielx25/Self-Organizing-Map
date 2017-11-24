#ifndef VALIDACION_H
#define VALIDACION_H
#include "arreglos.h"
#include "configuracion.h"
#include <math.h>
#include <limits>
#include "distancias.h"


class Validacion
{
    public:
        Validacion();
        virtual ~Validacion();

    int seleccionNeuronaGanadora();
	void setDatosEntrenamiento(double **datos);
	void setPesosRNA(double **datos, int numeroN);

	void iniciarValidacion();



    protected:
    private:

	double **datosEntrenamiento;
	double **pesosRNA;
	double *entrada;
	double *neurona;
    double radioMp10;
	//
	int numeroAciertos;
	//porcentaje de acertividad
	double acertividad;

	int numeroNeuronas;

};

#endif // VALIDACION_H

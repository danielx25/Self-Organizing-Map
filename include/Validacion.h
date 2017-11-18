#ifndef VALIDACION_H
#define VALIDACION_H


class Validacion
{
    public:
        Validacion();
        virtual ~Validacion();

	void setDatosEntrenamiento(double **datos);
	void setPesosRNA(double **datos);

	void iniciarValidacion();

	

    protected:
    private:

	double **datosEntrenamiento;
	double **pesosRNA;
	double *entrada;
		
	//
	double numeroAciertos;
	//porcentaje de acertividad
	double acertividad;
	
};

#endif // VALIDACION_H

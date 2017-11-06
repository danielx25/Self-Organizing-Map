#include "gestionadorsom.h"

GestionadorSOM::GestionadorSOM()
{
    //ctor
}

GestionadorSOM::~GestionadorSOM()
{
    //dtor
}

static double ** createByteMatrix(unsigned int rows, unsigned int cols)
{
    // declaration
	double ** a;
	// allocation
	a = new double*[rows];
	for(int i = 0; i < rows; i++)
		a[i] = new double[cols];
    return a;
}

void GestionadorSOM::inicializarSOM(double **datosEntrenamiento1, int numeroDatos)
{
    datosEntrenamiento = datosEntrenamiento1;
    som1 = new SOM(datosEntrenamiento1);
    FicheroRNA::crearConfiguracion();
    FicheroRNA::guardarCSV(datosEntrenamiento);// en caso de seguridad

}

/**el largo debe ser un numero par*/
void GestionadorSOM::inicializarSOM(double **datosEntrenamiento1,int numeroDatos, int alfa, int beta, int numeroNeuronas, int ancho, int largo)
{
    datosEntrenamiento = datosEntrenamiento1;
    som1 = new SOM(datosEntrenamiento1);
    Configuracion::ALFA = alfa;
    Configuracion::BETA = beta;
    Configuracion::NUMERO_DATOS = numeroDatos;
    Configuracion::NUMERO_NEURONAS = numeroNeuronas;
    Configuracion::ANCHO = ancho;
    Configuracion::LARGO = largo;
    FicheroRNA::crearConfiguracion();
    FicheroRNA::guardarCSV(datosEntrenamiento);// en caso de seguridad
}


void GestionadorSOM::empezarEntrenamiento()
{
    som1->entrenamiento();
}

void GestionadorSOM::reanudarEntrenamiento()
{
    FicheroRNA::leerConfiguracion();
    createByteMatrix(datosEntrenamiento, Configuracion::NUMERO_DATOS, Configuracion::NUMERO_ENTRADAS);
    som1 = new SOM();
    if(!FicheroRNA::leerPesosRNA(som1->getRedNeuronal()))
        som1->pesosAleatorios();
}

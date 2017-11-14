#include "gestionadorsom.h"

GestionadorSOM::GestionadorSOM()
{
    //ctor
}

GestionadorSOM::~GestionadorSOM()
{
    //dtor
}

static void *proceso_hilos(void *funcion)
{
    SOM *som1 = (SOM*)funcion;
    som1->entrenamiento();
    pthread_exit(NULL);
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

void GestionadorSOM::inicializarSOM(double **datosEntrenamiento1, int numeroDatos, int numEntrada)
{
    FicheroRNA::leerConfiguracion();
    datosEntrenamiento = datosEntrenamiento1;
    som1 = new SOM(datosEntrenamiento1);
    Configuracion::NUMERO_ENTRADAS = numEntrada;
    Configuracion::NUMERO_DATOS = numeroDatos;
    FicheroRNA::crearConfiguracion();
    FicheroRNA::guardarCSV(datosEntrenamiento);// en caso de seguridad

}

/**el largo debe ser un numero par*/
void GestionadorSOM::inicializarSOM(double **datosEntrenamiento1,int numeroDatos, int numEntrada, int alfa, int beta, int numeroNeuronas, int ancho, int largo)
{
    datosEntrenamiento = datosEntrenamiento1;
    som1 = new SOM(datosEntrenamiento1);
    Configuracion::ALFA = alfa;
    Configuracion::BETA = beta;
    Configuracion::NUMERO_ENTRADAS = numEntrada;
    Configuracion::NUMERO_DATOS = numeroDatos;
    Configuracion::NUMERO_NEURONAS = numeroNeuronas;
    Configuracion::ANCHO = ancho;
    Configuracion::LARGO = largo;
    FicheroRNA::crearConfiguracion();
    FicheroRNA::guardarCSV(datosEntrenamiento);// en caso de seguridad
}

void GestionadorSOM::statusProgresoRNA()
{
    progreso = (som1->iteracion)/(double)(som1->numeroIteraciones*Configuracion::NUMERO_DATOS);
}


void GestionadorSOM::empezarEntrenamiento()
{
    pthread_create(&hilo, NULL, proceso_hilos, (void*)som1);

    while(som1->iteracion < som1->numeroIteraciones*Configuracion::NUMERO_DATOS)
    {
        statusProgresoRNA();
        Sleep(tiempoGuardadoRed);
        som1->setPausar(true);
        while(som1->getListoGuardar() == false)
        {
        }
        if(som1->getListoGuardar())
        {
            guardarEstadoRNA();
        }
        som1->setPausar(false);
    }
}

void GestionadorSOM::reanudarEntrenamiento()
{
    FicheroRNA::leerConfiguracion();
    datosEntrenamiento = createByteMatrix(Configuracion::NUMERO_DATOS, Configuracion::NUMERO_ENTRADAS);
    som1 = new SOM(datosEntrenamiento);

    if(FicheroRNA::leerStatusRNA(som1))
    {
        //si no hay archivo de pesos deberia resetearse
        FicheroRNA::leerPesosRNA(som1->getRedNeuronal());
    }

}

void GestionadorSOM::guardarEstadoRNA()
{
    FicheroRNA::escribirJS(Configuracion::ANCHO, Configuracion::LARGO, som1->getMapaHex(), som1->getRedNeuronal());
    FicheroRNA::guardarPesosRNA(som1->getRedNeuronal());
    FicheroRNA::guardarStatusRNA(som1);
}

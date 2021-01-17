#ifndef NEURONAHEX_H_INCLUDED
#define NEURONAHEX_H_INCLUDED

#include "configuracion.h"
#include <stdio.h>




struct conexion
{
    double distancia;
    bool conectado;
    int fila;
    int columna;
};

typedef struct
{
    int id;
    double numero_activaciones;
    double *pesos = new double[Configuracion::NUMERO_ENTRADAS];
    int x;
    int y;
    conexion lado1;//derecha
    conexion lado2;//derecha-abajo
    conexion lado3;//izquier-abajo
    conexion lado4;//izquierd
    conexion lado5;//izquierd-arriba
    conexion lado6;//derecha-arriba

}  NeuronaHex;

static void inicNeuronaHex(NeuronaHex **mapaHex, int fila, int columna)
{
    mapaHex[fila][columna].id = -1;
    mapaHex[fila][columna].numero_activaciones = 0;
    mapaHex[fila][columna].lado1.columna = mapaHex[fila][columna].lado1.fila = -1;
    mapaHex[fila][columna].lado2.columna = mapaHex[fila][columna].lado2.fila = -1;
    mapaHex[fila][columna].lado3.columna = mapaHex[fila][columna].lado3.fila = -1;
    mapaHex[fila][columna].lado4.columna = mapaHex[fila][columna].lado4.fila = -1;
    mapaHex[fila][columna].lado5.columna = mapaHex[fila][columna].lado5.fila = -1;
    mapaHex[fila][columna].lado6.columna = mapaHex[fila][columna].lado6.fila = -1;
    mapaHex[fila][columna].lado1.conectado = false;
    mapaHex[fila][columna].lado2.conectado = false;
    mapaHex[fila][columna].lado3.conectado = false;
    mapaHex[fila][columna].lado4.conectado = false;
    mapaHex[fila][columna].lado5.conectado = false;
    mapaHex[fila][columna].lado6.conectado = false;
}
static void mostrarConxHex(NeuronaHex **mapaHex)
{
    for(int fila=0; fila<Configuracion::ANCHO; fila++)
    {
        for(int columna=0; columna<Configuracion::LARGO; columna++)
        {
            printf("[%d, %d] = (%d, %d) | ", fila, columna, mapaHex[fila][columna].lado6.fila, mapaHex[fila][columna].lado6.columna);
        }
        printf("\n");
    }
}


//�odd-r� horizontal layout
static void crearMatrizConexionHex(NeuronaHex **mapaHex)
{
    /*
    NeuronaHex **mapaHex;//[Configuracion::ANCHO][Configuracion::LARGO];
    redNeuronal=new double*[Configuracion::NUMERO_NEURONAS];
    for(int i=0; i<Configuracion::NUMERO_NEURONAS; i++)
        redNeuronal[i] = new double[Configuracion::NUMERO_ENTRADAS];


    mapaHex = new NeuronaHex*[Configuracion::ANCHO];
    for(int i=0; i<Configuracion::ANCHO; i++)
        mapaHex[i] = new NeuronaHex[Configuracion::LARGO];
        */

    int genID = 0;
    for(int fila=0; fila<Configuracion::ANCHO; fila++)
    {
        for(int columna=0; columna<Configuracion::LARGO; columna++)
        {
            //inicNeuronaHex(mapaHex, fila, columna);
            mapaHex[fila][columna].numero_activaciones = 0;
            mapaHex[fila][columna].lado1.columna = mapaHex[fila][columna].lado1.fila = -1;
            mapaHex[fila][columna].lado2.columna = mapaHex[fila][columna].lado2.fila = -1;
            mapaHex[fila][columna].lado3.columna = mapaHex[fila][columna].lado3.fila = -1;
            mapaHex[fila][columna].lado4.columna = mapaHex[fila][columna].lado4.fila = -1;
            mapaHex[fila][columna].lado5.columna = mapaHex[fila][columna].lado5.fila = -1;
            mapaHex[fila][columna].lado6.columna = mapaHex[fila][columna].lado6.fila = -1;
            mapaHex[fila][columna].lado1.conectado = false;
            mapaHex[fila][columna].lado2.conectado = false;
            mapaHex[fila][columna].lado3.conectado = false;
            mapaHex[fila][columna].lado4.conectado = false;
            mapaHex[fila][columna].lado5.conectado = false;
            mapaHex[fila][columna].lado6.conectado = false;

            mapaHex[fila][columna].id = genID;
            genID++;
        }
    }
    int par = -1;
    for(int fila=0; fila<Configuracion::ANCHO; fila++)
    {
        for(int columna=0; columna<Configuracion::LARGO; columna++)
        {
            if(mapaHex[fila][columna].lado1.conectado == false)
            {
                if(columna+1 < Configuracion::LARGO)
                    mapaHex[fila][columna].lado1.columna = columna+1;
                else
                    mapaHex[fila][columna].lado1.columna = 0;

                mapaHex[fila][columna].lado1.fila = fila;
                mapaHex[fila][columna].lado1.conectado=true;

            }

            if(mapaHex[fila][columna].lado2.conectado == false)
            {

                //estas conexiones dependen de la fila que se encuentra si es par o impar
                if(fila+1 < Configuracion::ANCHO && columna+1 < Configuracion::LARGO)
                {
                    if(par == -1)//si la posicion que ocupa la fila es impar
                    {
                        mapaHex[fila][columna].lado2.fila=fila+1;
                        mapaHex[fila][columna].lado2.columna=columna;
                    }
                    else
                    {
                        mapaHex[fila][columna].lado2.fila=fila+1;
                        mapaHex[fila][columna].lado2.columna=columna+1;
                    }
                }

                if(fila+1 < Configuracion::ANCHO && columna+1 == Configuracion::LARGO)
                {
                    if(par == 1)
                    {
                        mapaHex[fila][columna].lado2.fila = fila+1;
                        mapaHex[fila][columna].lado2.columna = 0;
                    }
                    else
                    {
                        mapaHex[fila][columna].lado2.fila = fila+1;
                        mapaHex[fila][columna].lado2.columna=columna;
                    }
                }

                if(fila+1 == Configuracion::ANCHO && columna+1 < Configuracion::LARGO)
                {
                    mapaHex[fila][columna].lado2.fila = 0;
                    mapaHex[fila][columna].lado2.columna = columna+1;

                }

                if(fila+1 == Configuracion::ANCHO && columna+1== Configuracion::LARGO)
                {
                    mapaHex[fila][columna].lado2.fila = 0;
                    mapaHex[fila][columna].lado2.columna = 0;
                }
                mapaHex[fila][columna].lado2.conectado=true;
            }

            if(mapaHex[fila][columna].lado3.conectado == false)
            {
                if(fila+1 < Configuracion::ANCHO && columna > 0)
                {
                    if(par == -1)
                    {
                        mapaHex[fila][columna].lado3.fila = fila +1;
                        mapaHex[fila][columna].lado3.columna = columna -1;
                    }
                    else
                    {
                        mapaHex[fila][columna].lado3.fila = fila +1;
                        mapaHex[fila][columna].lado3.columna = columna;
                    }
                }

                if(fila+1 < Configuracion::ANCHO && columna == 0)
                {
                    if(par == -1)
                    {
                        mapaHex[fila][columna].lado3.fila = fila+1;
                        mapaHex[fila][columna].lado3.columna = Configuracion::LARGO -1;
                    }
                    else
                    {
                        mapaHex[fila][columna].lado3.fila = fila +1;
                        mapaHex[fila][columna].lado3.columna = columna;
                    }
                }

                if(fila+1 == Configuracion::ANCHO && columna >= 0)
                {
                    mapaHex[fila][columna].lado3.fila=0;
                    mapaHex[fila][columna].lado3.columna=columna;
                }
                mapaHex[fila][columna].lado3.conectado = true;
            }
            if(mapaHex[fila][columna].lado4.conectado == false)
            {
                if(columna == 0)
                    mapaHex[fila][columna].lado4.columna=Configuracion::LARGO-1;
                else
                    mapaHex[fila][columna].lado4.columna=columna-1;
                mapaHex[fila][columna].lado4.fila=fila;
                mapaHex[fila][columna].lado4.conectado = true;
            }

            if(mapaHex[fila][columna].lado5.conectado == false)
            {
                if(fila > 0 && columna > 0)
                {
                    if(par == -1)
                    {
                        mapaHex[fila][columna].lado5.fila=fila-1;
                        mapaHex[fila][columna].lado5.columna=columna-1;
                    }
                    else
                    {
                        mapaHex[fila][columna].lado5.fila=fila-1;
                        mapaHex[fila][columna].lado5.columna=columna;
                    }
                }

                if(fila == 0 && columna > 0)
                {
                    mapaHex[fila][columna].lado5.fila=Configuracion::ANCHO-1;
                    mapaHex[fila][columna].lado5.columna=columna-1;
                }

                if(fila >0 && columna == 0)
                {
                    if(par == -1)
                    {
                        mapaHex[fila][columna].lado5.fila=fila-1;
                        mapaHex[fila][columna].lado5.columna=Configuracion::LARGO-1;
                    }
                    else
                    {
                        mapaHex[fila][columna].lado5.fila=fila-1;
                        mapaHex[fila][columna].lado5.columna=0;
                    }
                }
                if(fila ==0 && columna == 0)
                {
                    mapaHex[fila][columna].lado5.fila=Configuracion::ANCHO-1;
                    mapaHex[fila][columna].lado5.columna=Configuracion::LARGO-1;
                }
                mapaHex[fila][columna].lado5.conectado = true;
            }
            if(mapaHex[fila][columna].lado6.conectado == false)
            {
                if(fila > 0 && columna +1 < Configuracion::LARGO)
                {
                    if(par == -1)
                    {
                        mapaHex[fila][columna].lado6.fila = fila -1;
                        mapaHex[fila][columna].lado6.columna = columna;
                    }
                    else
                    {
                        mapaHex[fila][columna].lado6.fila = fila -1;
                        mapaHex[fila][columna].lado6.columna = columna +1;
                    }
                }

                if(fila > 0 && columna == Configuracion::LARGO-1)
                {
                    if(par ==-1)
                    {
                        mapaHex[fila][columna].lado6.fila = fila -1;
                        mapaHex[fila][columna].lado6.columna = columna;
                    }
                    else
                    {
                        mapaHex[fila][columna].lado6.fila = fila -1;
                        mapaHex[fila][columna].lado6.columna = 0;

                    }
                }

                if(fila == 0)
                {
                    mapaHex[fila][columna].lado6.fila = Configuracion::ANCHO-1;
                    mapaHex[fila][columna].lado6.columna = columna;
                }
                mapaHex[fila][columna].lado6.conectado = true;
            }
        }
        par*=-1;
    }
    //mostrarConxHex(mapaHex);
    /*
     for(int fila=0; fila<Configuracion::ANCHO; fila++)
    {
        for(int columna=0; columna<Configuracion::LARGO; columna++)
        {
            printf("[%d, %d] = (%d, %d) | ", fila, columna, mapaHex[fila][columna].lado6.fila, mapaHex[fila][columna].lado6.columna);
        }
        printf("\n");
    }*/


}



static void mapeo(int indiceNeurona)
{
    int x = indiceNeurona/Configuracion::LARGO;
    int y = indiceNeurona%Configuracion::LARGO;

}

#endif // NEURONAHEX_H_INCLUDED

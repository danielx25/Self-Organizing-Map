#ifndef NEURONA_H
#define NEURONA_H

#include "som.h"
#include "configuracion.h"
#include <stdio.h>

/*
struct conexion
{
    bool conectado;
    int fila;
    int columna;
};

typedef struct
{
    int id;
    int numero_activaciones;
    double pesos[Configuracion::NUMERO_ENTRADAS];

    conexion lado1;//derecha
    conexion lado2;//derecha-abajo
    conexion lado3;//izquier-abajo
    conexion lado4;//izquierd
    conexion lado5;//izquierd-arriba
    conexion lado6;//derecha-arriba

}  NeuronaHex;

static NeuronaHex inicNeuronaHex()
{
    NeuronaHex neu;
    neu.id = -1;
    neu.numero_activaciones -1;
    neu.lado1.columna = neu.lado1.fila = -1;
    neu.lado2.columna = neu.lado2.fila = -1;
    neu.lado3.columna = neu.lado3.fila = -1;
    neu.lado4.columna = neu.lado4.fila = -1;
    neu.lado5.columna = neu.lado5.fila = -1;
    neu.lado6.columna = neu.lado6.fila = -1;
    neu.lado1.conectado = false;
    neu.lado2.conectado = false;
    neu.lado3.conectado = false;
    neu.lado4.conectado = false;
    neu.lado5.conectado = false;
    neu.lado6.conectado = false;
    return neu;
}
//“odd-r” horizontal layout
static void crearMatrizConexionHex(NeuronaHex mapaHex[Configuracion::ANCHO][Configuracion::LARGO])
{
    int genID = 0;
    for(int fila=0; fila<Configuracion::ANCHO; fila++)
    {
        for(int columna=0; columna<Configuracion::LARGO; columna++)
        {
            mapaHex[fila][columna] = inicNeuronaHex();
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


     for(int fila=0; fila<Configuracion::ANCHO; fila++)
    {
        for(int columna=0; columna<Configuracion::LARGO; columna++)
        {
            printf("[%d, %d] = (%d, %d) | ", fila, columna, mapaHex[fila][columna].lado6.fila, mapaHex[fila][columna].lado6.columna);
        }
        printf("\n");
    }


}

static void mapeo(int indiceNeurona)
{
    int x = indiceNeurona/Configuracion::LARGO;
    int y = indiceNeurona%Configuracion::LARGO;

}


class neurona
{
    public:
        neurona();
        virtual ~neurona();
        neurona * siguiente;
        neurona * anterior;

    protected:

    private:
        int id;
        int numeroPesos;
        double pesos[Configuracion::NUMERO_NEURONAS];
        int numeroActivacion;
        bool detener;
};
*/
#endif // NEURONA_H

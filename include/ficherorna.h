#ifndef FICHERORNA_H
#define FICHERORNA_H


#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <stdlib.h>
#include <sstream>
#include "NeuronaHex.h"

const char NOMBRE_FICHERO[100] = "valores.js";

class FicheroRNA
{
    public:
        FicheroRNA();
        virtual ~FicheroRNA();

        static std::string getexepath()
        {
          char result[ MAX_PATH ];
          return std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
        }



        static void escribirJS(int ancho , int largo, NeuronaHex mapaHex[Configuracion::ANCHO][Configuracion::LARGO])
        {
            std::string cadena;
            std::stringstream ss2;

            ss2 << ancho;
            cadena =  "var ancho = "+ss2.str()+ ";\n";
            ss2.str("");
            ss2<<largo;
            cadena += "var largo = "+ss2.str()+ ";\n\n";

            cadena+="var lista_red = [";

            for(int fila = 0; fila <ancho; fila++)
            {
                for(int columna =0; columna<largo; columna++)
                {
                    ss2.str("");
                    ss2<<mapaHex[fila][columna].numero_activaciones;
                    cadena+= ss2.str()+", ";
                }
                cadena+="\n";
            }
            cadena+="];\n";
            std::ofstream fichero(NOMBRE_FICHERO, std::ios::ate);
            fichero << cadena;
            fichero.close();
        }

    protected:

    private:
};

#endif // FICHERORNA_H

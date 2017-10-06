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



        static void escribirJS(int ancho , int largo, NeuronaHex mapaHex[Configuracion::ANCHO][Configuracion::LARGO], double redNeuronal[][Configuracion::NUMERO_NEURONAS])
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
            cadena+="];\n\n";
            ss2.str("");
            ss2<<Configuracion::NUMERO_NEURONAS;
            cadena+="var numeroNeurona= "+ss2.str()+"\n";
            ss2.str("");
            ss2<<Configuracion::NUMERO_ENTRADAS;
            cadena+="var numeroEntrada= "+ss2.str()+"\n";
            cadena+="var lista_Pesos= [ \n";
            for(int indiceNeurona =0; indiceNeurona<Configuracion::NUMERO_NEURONAS; indiceNeurona++)
            {
                cadena+="[ ";
                for(int indicePeso=0; indicePeso<Configuracion::NUMERO_ENTRADAS; indicePeso++)
                {
                    ss2.str("");
                    ss2<<redNeuronal[indicePeso][indiceNeurona];
                    cadena+=ss2.str()+ ", ";

                }
                cadena+="],\n";
            }
            cadena+="];\n";
            std::ofstream fichero(NOMBRE_FICHERO, std::ios::ate);
            fichero << cadena;
            fichero.close();
        }

        static void leerCSV(std::string rutaArchivo,double **datosEntrenamiento )
        {
            std::ifstream in(rutaArchivo);
            std::vector<std::vector<double>> fields;
            if (in) {
                std::string line;
                while (getline(in, line)) {
                    std::stringstream sep(line);
                    std::string field;
                    fields.push_back(std::vector<double>());
                    while (getline(sep, field, ';')) {
                        fields.back().push_back(atof(field.c_str()));
                    }
                }
            }

            std::cout << "hollaaaaa ";
            int fila = 0;
            int columna = 0;
            //printf("numero filas: %d\n", fields.size());
            for (auto row : fields) {
                //printf("numero filas: %d\n", row.size());
                columna = 0;
                for (auto field : row) {
                    //printf("[%d, %d]\n", fila, columna);
                    datosEntrenamiento[fila][columna] = field;
                    columna+=1;
                }
                fila+=1;
            }

        }

    protected:

    private:
};

#endif // FICHERORNA_H

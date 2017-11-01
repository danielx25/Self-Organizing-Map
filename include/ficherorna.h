#ifndef FICHERORNA_H
#define FICHERORNA_H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <stdlib.h>
#include <sstream>
#include "configuracion.h"
#include "NeuronaHex.h"
#include "som.h"

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



        static void escribirJS(int ancho , int largo, NeuronaHex **mapaHex, double **redNeuronal)
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
                    //printf("ancho: %d largo: %d\n", fila, columna);
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

        static void guardarPesosRNA (double **redNeuronal)
        {
            std::string cadena;
            std::stringstream ss2;

            ss2.str("");

            //cadena+="var lista_Pesos= [ \n";
            for(int indiceNeurona =0; indiceNeurona<Configuracion::NUMERO_NEURONAS; indiceNeurona++)
            {
                //cadena+="[ ";
                for(int indicePeso=0; indicePeso<Configuracion::NUMERO_ENTRADAS; indicePeso++)
                {
                    ss2.str("");
                    ss2<<redNeuronal[indicePeso][indiceNeurona];
                    cadena+=ss2.str()+ "; ";

                }
                cadena+="\n";
            }
            //cadena+="];\n";
            std::ofstream fichero("pesosRNA.csv", std::ios::ate);
            fichero << cadena;
            fichero.close();
        }

        static bool leerCSV(std::string rutaArchivo,double **datosEntrenamiento )
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
                in.close();
            }
            else
                return false;

            int fila = 0;
            int columna = 0;
            for (auto row : fields) {
                columna = 0;
                for (auto field : row) {
                    datosEntrenamiento[fila][columna] = field;
                    columna+=1;
                }
                fila+=1;
            }
            return true;
        }
        /*
        static void guardarEstadoRed(SOM som2)
        {
            std::ofstream salida;
            salida.open("Som.dat", std::ios::binary|std::ios::app);
            salida.write(&som2,sizeof(som2));
            salida.close();
        }

        static void leerEstadoRed()
        {
            std::ifstream entrada;
            //Cliente cliente;
            entrada.open("Som.dat", std::ios::binary|std::ios::app);
            entrada.seekg(0,std::ios::end);
            long longitudFichero=entrada.tellg();
            entrada.seekg(0, std::ios::beg);
        }*/
        static void crearConfiguracion()
        {
            std::string cadena;
            std::stringstream ss2;

            ss2.str("");
            ss2<<Configuracion::NUMERO_ENTRADAS;
            cadena = "NUMERO_ENTRADAS = "+ss2.str()+"\n";

            ss2.str("");
            ss2<<Configuracion::NUMERO_DATOS;
            cadena += "NUMERO_DATOS = "+ss2.str()+"\n";

            ss2.str("");
            ss2<<Configuracion::NUMERO_NEURONAS;
            cadena += "NUMERO_NEURONAS = "+ss2.str()+"\n";

            ss2.str("");
            ss2<<Configuracion::LARGO;
            cadena += "LARGO = "+ss2.str()+"\n";

            ss2.str("");
            ss2<<Configuracion::ANCHO;
            cadena += "el largo puede ser cualquier pero el ancho tiene que ser par(para que la estructura hexagonal pueda unirse en sus limites)\n";
            cadena +="como un balon de futbol con caras hexagonales\n";
            cadena += "ANCHO = "+ss2.str()+"\n";

            ss2.str("");
            ss2<<Configuracion::ALFA;
            cadena += "ALFA = "+ss2.str()+"\n";

            ss2.str("");
            ss2<<Configuracion::BETA;
            cadena += "BETA = "+ss2.str()+"\n";

            ss2.str("");
            ss2<<Configuracion::RANGO_VECINDAD;
            cadena += "RANGO_VECINDAD = "+ss2.str()+"\n";


            std::ofstream fichero("ConfiguracionRNA.conf", std::ios::ate);
            fichero << cadena;
            fichero.close();
        }
    protected:

    private:
};

#endif // FICHERORNA_H

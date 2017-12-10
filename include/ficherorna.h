#ifndef FICHERORNA_H
#define FICHERORNA_H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <windows.h>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include "configuracion.h"
#include "NeuronaHex.h"
#include "som.h"
#include "arreglos.h"

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
            for(int indiceNeurona =0; indiceNeurona<Configuracion::NUMERO_NEURONAS; indiceNeurona++)
            {
                for(int indicePeso=0; indicePeso<Configuracion::NUMERO_ENTRADAS; indicePeso++)
                {
                    ss2.str("");
                    ss2<<redNeuronal[indicePeso][indiceNeurona];
                    cadena+=ss2.str();
                    if(indicePeso != Configuracion::NUMERO_ENTRADAS-1)
                    cadena+="; ";

                }
                cadena+="\n";
            }
            std::ofstream fichero("pesosRNA.csv", std::ios::ate);
            fichero << cadena;
            fichero.close();
        }

        static bool leerPesosRNA(double **datosEntrenamiento )
        {
            std::ifstream in("pesosRNA.csv");
            std::vector<std::vector<double>> fields;
            if (in) {
                std::string line;

                while (getline(in, line)) {
                    std::stringstream sep(line);
                    std::string field;
                    fields.push_back(std::vector<double>());
                    while (getline(sep, field, ';')) {
                        //field = RemoveChar(field, ' ');
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
                    datosEntrenamiento[columna][fila] = field;
                    columna+=1;
                }
                fila+=1;
            }
            return true;
        }

        static void guardarCSV (double **datosEntrenamiento)
        {
            std::string cadena;
            std::stringstream ss2;

            ss2.str("");
            for(int indiceNeurona =0; indiceNeurona<Configuracion::NUMERO_DATOS; indiceNeurona++)
            {
                for(int indicePeso=0; indicePeso<Configuracion::NUMERO_ENTRADAS; indicePeso++)
                {
                    ss2.str("");
                    ss2<<datosEntrenamiento[indiceNeurona][indicePeso];
                    cadena+=ss2.str();

                    if(indicePeso != Configuracion::NUMERO_ENTRADAS-1)
                    cadena+="; ";

                }
                cadena+="\n";
            }
            std::ofstream fichero("aux.csv", std::ios::ate);//((Configuracion::RUTA_ARCHIVO, std::ios::ate);
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
            {
                printf("Fichero no encontrado\n");
                return false;
            }


            int fila = 0;
            int columna = 0;

            printf("fila: %d\n", fields.size());
            printf("colm,: %d\n", fields[0].size());

            Configuracion::NUMERO_DATOS = fields.size();
            datosEntrenamiento = Arreglos::createByteMatrix(fields.size(), fields[0].size());
            if(fields[0].size()==Configuracion::NUMERO_ENTRADAS)
            {
                for (auto row : fields)
                {

                    columna = 0;
                    for (auto field : row) {
                        datosEntrenamiento[fila][columna] = field;
                        columna+=1;
                    }
                    fila+=1;
                }

                for(int fila = 0;  fila<10; fila++)
                {
                    for(int columna = 0; columna<Configuracion::NUMERO_ENTRADAS; columna++)
                    {
                        printf("| %3f |", datosEntrenamiento[fila][columna]);
                    }
                    printf("\n");
                }


            }
            else
            {
                printf("Error Archivo Configuracion: El numero de columnas del archivo no coincide con el numero entrada.\n");
                return false;
            }

            return true;
        }

        static std::string RemoveChar(std::string str, char c)
        {
           std::string result;
           for (size_t i = 0; i < str.size(); i++)
           {
                  char currentChar = str[i];
                  if (currentChar != c)
                      result += currentChar;
           }
               return result;
        }

        static bool leerConfiguracion()
        {
            std::ifstream in("ConfiguracionRNA.conf");
            if (in) {
                std::string line;

                bool rutaArchivo = false;
                bool numeroEntrada = false;
                bool numeroDatos = false;
                bool numeroNeuronas = false;
                bool largo = false;
                bool ancho = false;
                bool beta = false;
                bool rangoVecindad = false;

                while (getline(in, line)) {
                    std::stringstream sep(line);
                    std::string field;


                    int contador = 0;
                    while (getline(sep, field, '=')) {
                        if(contador == 1)
                        {
                            if (line.find("RUTA_ARCHIVO") != std::string::npos)
                            {
                                rutaArchivo = true;
                                field = RemoveChar(field, ' ');
                                Configuracion::RUTA_ARCHIVO = field;
                            }

                            if (line.find("NUMERO_ENTRADAS") != std::string::npos)
                            {
                                numeroEntrada = true;
                                field = RemoveChar(field, ' ');
                                Configuracion::NUMERO_ENTRADAS = atof(field.c_str());
                            }

                            if (line.find("NUMERO_DATOS") != std::string::npos)
                            {
                                numeroEntrada = true;
                                field = RemoveChar(field, ' ');
                                //Configuracion::NUMERO_DATOS = atof(field.c_str());
                            }

                            if (line.find("NUMERO_NEURONAS") != std::string::npos)
                            {
                                numeroEntrada = true;
                                field = RemoveChar(field, ' ');
                                Configuracion::NUMERO_NEURONAS = atof(field.c_str());
                            }

                            if (line.find("LARGO") != std::string::npos)
                            {
                                numeroEntrada = true;
                                field = RemoveChar(field, ' ');
                                Configuracion::LARGO = atof(field.c_str());
                            }

                            if (line.find("ANCHO") != std::string::npos)
                            {
                                numeroEntrada = true;
                                field = RemoveChar(field, ' ');
                                Configuracion::ANCHO = atof(field.c_str());
                            }

                            if (line.find("ALFA") != std::string::npos)
                            {
                                numeroEntrada = true;
                                field = RemoveChar(field, ' ');
                                Configuracion::ALFA = atof(field.c_str());
                            }

                            if (line.find("BETA") != std::string::npos)
                            {
                                numeroEntrada = true;
                                field = RemoveChar(field, ' ');
                                Configuracion::BETA = atof(field.c_str());
                            }

                            if (line.find("RANGO_VECINDAD") != std::string::npos)
                            {
                                numeroEntrada = true;
                                field = RemoveChar(field, ' ');
                                Configuracion::RANGO_VECINDAD = atof(field.c_str());
                            }

                            if (line.find("PESO_DIMENSION_OBJ") != std::string::npos)
                            {
                                numeroEntrada = true;
                                field = RemoveChar(field, ' ');
                                Configuracion::PESO_DIMENSION_OBJ = atof(field.c_str());
                            }
                        }
                        contador+=1;
                    }
                }
                in.close();
                return true;
            }
            else
                return false;
        }
        static void crearConfiguracion()
        {
            std::string cadena;
            std::stringstream ss2;

            cadena = "RUTA_ARCHIVO = "+Configuracion::RUTA_ARCHIVO+"\n";

            ss2.str("");
            ss2<<Configuracion::NUMERO_ENTRADAS;
            cadena += "NUMERO_ENTRADAS = "+ss2.str()+"\n";

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

            ss2.str("");
            ss2<<Configuracion::PESO_DIMENSION_OBJ;
            cadena += "PESO_DIMENSION_OBJ = "+ss2.str()+"\n";



            std::ofstream fichero("ConfiguracionRNA.conf", std::ios::ate);
            fichero << cadena;
            fichero.close();
        }

        static void guardarStatusRNA(SOM *som1)
        {
            std::string cadena;
            std::stringstream ss2;

            if (som1->getTerminoEntrenarse())
                cadena = "termino_entrenarse = si\n";
            else
                cadena = "termino_entrenarse = no\n";


            ss2.str("");
            ss2<<som1->ciclos;
            cadena += "ciclos = "+ss2.str()+"\n";

            ss2.str("");
            ss2<<som1->iteracion;
            cadena += "iteracion = "+ss2.str()+"\n";

            ss2.str("");
            ss2<<som1->getAlfa();
            cadena += "alfa = "+ss2.str()+"\n";

            ss2.str("");
            ss2<<som1->getBeta();
            cadena += "beta = "+ss2.str()+"\n";

            //ss2.str("");
            //ss2<<Configuracion::LARGO;
            cadena += "alfas = [";

            for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
            {
                ss2.str("");
                ss2<<som1->getAlfas()[i];
                if (i != Configuracion::NUMERO_ENTRADAS-1)
                    cadena += ss2.str()+", ";
                else
                    cadena += ss2.str()+"]\n";
            }

            cadena += "betas = [";

            for(int i=0; i<Configuracion::NUMERO_ENTRADAS; i++)
            {
                ss2.str("");
                ss2<<som1->getBetas()[i];
                if (i != Configuracion::NUMERO_ENTRADAS-1)
                    cadena += ss2.str()+", ";
                else
                    cadena += ss2.str()+"]";
            }

            std::ofstream fichero("statusRNA.dat", std::ios::ate);
            fichero << cadena;
            fichero.close();
        }


        static bool leerStatusRNA(SOM *som1)
        {
            std::ifstream in("statusRNA.dat");
            if (in) {
                std::string line;

                while (getline(in, line)) {
                    std::stringstream sep(line);
                    std::string field;


                    int contador = 0;
                    while (getline(sep, field, '=')) {
                        if(contador == 1)
                        {
                            if (line.find("termino_entrenarse") != std::string::npos)
                            {
                                field = RemoveChar(field, ' ');
                                if(field.find("si") != std::string::npos)
                                    som1->setTerminoEntrenarse(true);
                                else
                                    som1->setTerminoEntrenarse(false);
                            }

                            if (line.find("ciclos") != std::string::npos)
                            {
                                field = RemoveChar(field, ' ');
                                som1->ciclos =atof(field.c_str());
                            }

                            if (line.find("iteracion") != std::string::npos)
                            {
                                field = RemoveChar(field, ' ');
                                som1->iteracion = atof(field.c_str());
                            }

                            if (line.find("alfas") != std::string::npos)
                            {
                                field = RemoveChar(field, '[');
                                field = RemoveChar(field, ']');
                                std::stringstream sep1(field);
                                std::string field1;
                                int indice = 0;
                                while (getline(sep1, field1, ','))
                                {
                                    field1 = RemoveChar(field1, ' ');
                                    som1->getAlfas()[indice] =  atof(field1.c_str());
                                    indice+=1;
                                    //printf("alfa: %s\n", field1.c_str());
                                }

                            }

                            if (line.find("betas") != std::string::npos)
                            {
                                field = RemoveChar(field, '[');
                                field = RemoveChar(field, ']');
                                std::stringstream sep1(field);
                                std::string field1;
                                int indice = 0;
                                while (getline(sep1, field1, ','))
                                {
                                    field1 = RemoveChar(field1, ' ');
                                    som1->getBetas()[indice] =  atof(field1.c_str());
                                    indice+=1;
                                    //printf("beta: %s\n", field1.c_str());
                                }

                            }

                        }
                        contador+=1;
                    }
                }
                in.close();
                return true;
            }
            else
                return false;
        }
    protected:

    private:
};

#endif // FICHERORNA_H

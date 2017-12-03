#ifndef DISTANCIAS_H
#define DISTANCIAS_H


class Distancias
{
    public:
        Distancias() {}
        virtual ~Distancias() {}


        /**
        funciones de medicion de Diatancias entre dos puntos de n dimensiones
        */
        static double distanciaEuclidea(double *entrada, double *pesos)
        {
            double sumatoria = 0;

            for(int i=0; i < Configuracion::NUMERO_ENTRADAS; i++)
            {
                sumatoria+= pow(entrada[i]- pesos[i], 2);
            }
            return sqrt(sumatoria);
        }


        static double distanciaManhattan(double *entrada, double *pesos)
        {
            double sumatoria = 0;

            for(int i=0; i < Configuracion::NUMERO_ENTRADAS; i++)
            {
                sumatoria+= abs(entrada[i]- pesos[i]);
            }
            return sumatoria;
        }

        static double distanciaMinkowski(double *entrada, double *pesos, double p)
        {
            double sumatoria = 0;

            for(int i=0; i < Configuracion::NUMERO_ENTRADAS; i++)
            {
                sumatoria+= pow(entrada[i]- pesos[i], p);
            }
            return pow(sumatoria, 1/p);
        }

        /**establece un nivel de importancia mayor en algunas de la dimensiones*/
        static double distanciaEuclidea_1(double *entrada, double *pesos)
        {
            double sumatoria = 0;
            double peso_otras_dimensiones = (1/(double)Configuracion::NUMERO_ENTRADAS);
            peso_otras_dimensiones-=(Configuracion::PESO_DIMENSION_OBJ/(double)Configuracion::NUMERO_ENTRADAS);
            for(int i=0; i < Configuracion::NUMERO_ENTRADAS; i++)
            {
                if(i!=Configuracion::NUMERO_ENTRADAS-1)
                    sumatoria+= peso_otras_dimensiones*pow(entrada[i]- pesos[i], 2);
                else{
                    sumatoria+= (peso_otras_dimensiones+Configuracion::PESO_DIMENSION_OBJ)*pow(entrada[i]- pesos[i], 2);
                }
            }
            return sqrt(sumatoria);
        }



    protected:

    private:
};

#endif // DISTANCIAS_H

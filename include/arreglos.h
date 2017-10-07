#ifndef ARREGLOS_H
#define ARREGLOS_H


class Arreglos
{
    public:
        Arreglos() {}
        virtual ~Arreglos() {}

        static int len(double *arreglo)
        {
            return sizeof(arreglo)/sizeof(*arreglo);
        }

        static double fRand(double fMin, double fMax)
        {
            double f = (double)rand() / RAND_MAX;
            return fMin + f * (fMax - fMin);
        }

        static void getNeurona(double *arreglo1, double matriz[][Configuracion::NUMERO_NEURONAS], int columna)
        {
            for(int fila=0; fila<Configuracion::NUMERO_ENTRADAS; fila++)
            {
                arreglo1[fila] = matriz[fila][columna];
            }
        }

        static void getFila(double *arreglo1, double **matriz, int fila)
        {

            for(int columna=0; columna<Configuracion::NUMERO_ENTRADAS; columna++)
            {
                arreglo1[columna] =matriz[fila][columna];
            }


        }


    protected:

    private:
};

#endif // ARREGLOS_H

#ifndef CONFIGURACION_H
#define CONFIGURACION_H


class Configuracion
{
    public:
        Configuracion() {}
        virtual ~Configuracion() {}

        static const int NUMERO_ENTRADAS = 38;
        static const int NUMERO_NEURONAS = 2500;
        static const int RANGO_VECINDAD = 4;
        static const int NUMERO_DATOS = 62385;
        /**el largo puede ser cualquier pero el ancho tiene que ser par(para que la estructura hexagonal pueda unirse en sus limites)
        como un balon de futbol con caras hexagonales*/
        static const int ANCHO = 50;
        static const int LARGO = 50;
        //tasa de aprendizaje
        static constexpr double ALFA = 0.5;
        //decrecimiento de la tasa de aprendizaje
        static constexpr double BETA = 0.005;

    protected:

    private:
};


#endif // CONFIGURACION_H

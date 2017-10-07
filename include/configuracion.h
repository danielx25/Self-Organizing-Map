#ifndef CONFIGURACION_H
#define CONFIGURACION_H


class Configuracion
{
    public:
        Configuracion() {}
        virtual ~Configuracion() {}

        static const int NUMERO_ENTRADAS = 38;
        static const int NUMERO_NEURONAS = 1600;
        static const int RANGO_VECINDAD = 4;
        static const int NUMERO_DATOS = 62385;
        /**el largo puede ser cualquier pero el ancho tiene que ser par(para que la estructura hexagonal puede unirse en sus limites)
        como un balon de futbol con caras hexagonales*/
        static const int ANCHO = 40;
        static const int LARGO = 40;

    protected:

    private:
};

#endif // CONFIGURACION_H

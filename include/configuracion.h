#ifndef CONFIGURACION_H
#define CONFIGURACION_H


class Configuracion
{
    public:
        Configuracion() {}
        virtual ~Configuracion() {}

        static const int NUMERO_ENTRADAS = 4;
        static const int NUMERO_NEURONAS = 910;
        static const int RANGO_VECINDAD = 4;
        static const int NUMERO_DATOS = 1200;
        /**el largo puede ser cualquier pero el ancho tiene que ser par(para que la estructura hexagonal puede unirse en sus limites)
        como un balon de futbol con caras hexagonales*/
        static const int ANCHO = 26;
        static const int LARGO = 35;

    protected:

    private:
};

#endif // CONFIGURACION_H

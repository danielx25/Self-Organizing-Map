//
// Created by danie on 09-01-2021.
//
#include <math.h>
#include "som.h"
#include "dibujarfiguras.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_video.h>
#include <SDL_render.h>


#ifndef SOM__VISUALIZACION_H
#define SOM__VISUALIZACION_H

#define PI 3.1415926535897932384626433832795

class Visualizacion {
    public:
        Visualizacion(SOM *som);

        double  radianToAngule(double r){
            return r*180/PI;
        }

        double  anguleToRadian(double a){
            return a*(PI/180);
        }

        static double distanciaEntreDosPuntos(double x1, double y1, double x2, double y2)
        {
            return sqrt(pow(x2-x1, 2)+pow(y2-y1,2));
        }
        static double angulo_recta(double x1, double y1, double x2, double y2)
        {
            double xDiff = x2-x1;
            double yDiff = y2-y1;
            return atan2(yDiff, xDiff)*180.0/PI;
        }

        int loop();
        void imprimirMalla();
        void calcularDistanciaHex();
        void calcularPosicionesHex(int fila, int columna);
        // x, y son los parametros que se deben calcular, [p1x, p1y] punto 1, [p2x, p2y] punto 2, los 3 lados
        void calcularTercerPunto(double *x, double *y, double p1x, double p1y, double p2x, double p2y, double lp1, double lp2);
        bool Initialize();
        void Close();
    private:
        SDL_Window* window = NULL;
        SDL_Surface *screen = NULL;
        SDL_Renderer *renderer = NULL;
        SOM * som = NULL;

};


#endif //SOM__VISUALIZACION_H

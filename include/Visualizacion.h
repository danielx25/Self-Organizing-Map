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


class Visualizacion {
    public:
        Visualizacion(SOM *som);
        int loop();
        void imprimirMalla();
        void calcularDistanciaHex();
        void calcularPosicionesHex(int fila, int columna);
        bool Initialize();
        void Close();
    private:
        SDL_Window* window = NULL;
        SDL_Surface *screen = NULL;
        SDL_Renderer *renderer = NULL;
        SOM * som = NULL;

};


#endif //SOM__VISUALIZACION_H

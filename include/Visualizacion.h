//
// Created by danie on 09-01-2021.
//

#include "som.h"
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
        Visualizacion();
        int loop();
        bool Initialize();
        void Close();
    private:
        SDL_Window* window = NULL;
        SDL_Surface *screen = NULL;
        SDL_Renderer *renderer = NULL;

};


#endif //SOM__VISUALIZACION_H

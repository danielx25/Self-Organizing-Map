//
// Created by danie on 09-01-2021.
//

#include "../include/Visualizacion.h"

Visualizacion::Visualizacion() {

}

int Visualizacion::loop() {
    if(!Initialize())
    {
        return -1;
    }
    atexit(SDL_Quit);
    atexit(TTF_Quit);


    bool exit = false;
    SDL_Event e;
    while (!exit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                exit = true;
            }
            if(e.type == SDL_KEYDOWN){
                if (e.key.keysym.sym == SDLK_ESCAPE){
                    exit = true;
                }
                if (e.key.keysym.sym == SDLK_u){
                    printf("ventana 1-2\n");
                }

            }
            if(e.type == SDL_KEYUP){
                if (e.key.keysym.sym == SDLK_LSHIFT){
                }
            }

        }


        //-----------------------------------------------------------------
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0, 0, 0));

        SDL_UpdateWindowSurface(window);
    }
    //GestorContenido::guardarFiguras(gestion_objetos->getTablaFiguras());
    printf("eliminando textFrame\n");
    Close();


    return 0;
}

bool Visualizacion::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }
    if (TTF_Init() < 0) {
        // Handle error...
    }

    window = SDL_CreateWindow("Framework", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              400, 400, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        return false;
    }
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

    screen = SDL_GetWindowSurface(window);

    return true;
}

void Visualizacion::Close() {
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(screen);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

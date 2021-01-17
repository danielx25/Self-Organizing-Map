//
// Created by danie on 09-01-2021.
//

#include "../include/Visualizacion.h"

Visualizacion::Visualizacion(SOM *som) {
    this->som = som;
}

int Visualizacion::loop() {
    if(!Initialize())
    {
        return -1;
    }
    atexit(SDL_Quit);
    atexit(TTF_Quit);

    //this->calcularDistanciaHex();
    this->calcularPosicionesHex(4,4);
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

        this->imprimirMalla();

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

void Visualizacion::imprimirMalla() {
    figura f_aux;
    desc_figura df;
    int x_ = 10;
    int y_;
    for (int i = 0; i < Configuracion::ANCHO ; ++i) {
        x_+= 10;
        y_ = 10;
        for (int j = 0; j < Configuracion::LARGO; ++j) {
            y_+=10;
            f_aux = DibujarFiguras::crearPunto(i+x_, j+y_);
            df.fg = &f_aux;
            DibujarFiguras::mostrarFigura(screen, df);
        }
    }

}

void Visualizacion::calcularDistanciaHex() {
    NeuronaHex neurona;

    int fila ;//= indiceNeurona/Configuracion::LARGO;
    int columna ;//= indiceNeurona%Configuracion::LARGO;
    double *neuronaOrigen = new double[Configuracion::NUMERO_ENTRADAS];
    double *neuronaVecino = new double[Configuracion::NUMERO_ENTRADAS];
    double distancia;
    int indiceNeuronaVecina;

    for(int indiceNeu=0; indiceNeu< Configuracion::NUMERO_NEURONAS; indiceNeu++)
    {
        fila = indiceNeu/Configuracion::LARGO;
        columna = indiceNeu%Configuracion::LARGO;
        Arreglos::getNeurona(neuronaOrigen, som->redNeuronal, indiceNeu);

        printf("indice Neu: %d col: %d fila: %d\n", columna, fila, indiceNeu);
        //-------------------------------------------------------------------------------
        indiceNeuronaVecina = Configuracion::LARGO*som->mapaHex[fila][columna].lado1.fila +
                som->mapaHex[fila][columna].lado1.columna;
        printf("indice neurona: %d\n", indiceNeuronaVecina);

        Arreglos::getNeurona(neuronaVecino, som->redNeuronal, indiceNeuronaVecina);

        distancia = Distancias::distanciaEuclidea_1(neuronaOrigen, neuronaVecino);
        som->mapaHex[fila][columna].lado1.distancia = distancia;
        //---------------------------------------------------------------------------------
        indiceNeuronaVecina = Configuracion::LARGO*som->mapaHex[fila][columna].lado2.fila +
                              som->mapaHex[fila][columna].lado2.columna;
        printf("indice neurona: %d\n", indiceNeuronaVecina);
        Arreglos::getNeurona(neuronaVecino, som->redNeuronal, indiceNeuronaVecina);

        distancia = Distancias::distanciaEuclidea_1(neuronaOrigen, neuronaVecino);
        som->mapaHex[fila][columna].lado2.distancia = distancia;
        //---------------------------------------------------------------------------------
        indiceNeuronaVecina = Configuracion::LARGO*som->mapaHex[fila][columna].lado3.fila +
                              som->mapaHex[fila][columna].lado3.columna;
        printf("indice neurona: %d\n", indiceNeuronaVecina);
        Arreglos::getNeurona(neuronaVecino, som->redNeuronal, indiceNeuronaVecina);

        distancia = Distancias::distanciaEuclidea_1(neuronaOrigen, neuronaVecino);
        som->mapaHex[fila][columna].lado3.distancia = distancia;
        //---------------------------------------------------------------------------------
        indiceNeuronaVecina = Configuracion::LARGO*som->mapaHex[fila][columna].lado4.fila +
                              som->mapaHex[fila][columna].lado4.columna;
        printf("indice neurona: %d\n", indiceNeuronaVecina);
        Arreglos::getNeurona(neuronaVecino, som->redNeuronal, indiceNeuronaVecina);

        distancia = Distancias::distanciaEuclidea_1(neuronaOrigen, neuronaVecino);
        som->mapaHex[fila][columna].lado4.distancia = distancia;
        //---------------------------------------------------------------------------------
        indiceNeuronaVecina = Configuracion::LARGO*som->mapaHex[fila][columna].lado5.fila +
                              som->mapaHex[fila][columna].lado5.columna;
        printf("indice neurona: %d\n", indiceNeuronaVecina);
        Arreglos::getNeurona(neuronaVecino, som->redNeuronal, indiceNeuronaVecina);

        distancia = Distancias::distanciaEuclidea_1(neuronaOrigen, neuronaVecino);
        som->mapaHex[fila][columna].lado5.distancia = distancia;
        //---------------------------------------------------------------------------------
        indiceNeuronaVecina = Configuracion::LARGO*som->mapaHex[fila][columna].lado6.fila +
                              som->mapaHex[fila][columna].lado6.columna;
        printf("indice neurona: %d\n", indiceNeuronaVecina);
        Arreglos::getNeurona(neuronaVecino, som->redNeuronal, indiceNeuronaVecina);

        distancia = Distancias::distanciaEuclidea_1(neuronaOrigen, neuronaVecino);
        som->mapaHex[fila][columna].lado6.distancia = distancia;

    }
    int it =1;
}

void Visualizacion::calcularPosicionesHex(int fila, int columna) {
    NeuronaHex **mapaHex = som->mapaHex;
    int contador_neuronas = 0;
    int contador_lado;
    int iter = 1;
    int fila_v = som->mapaHex[fila][columna].lado1.fila;
    int columna_v = som->mapaHex[fila][columna].lado1.columna;
    double distancia;
    double *neuronaOrigen = new double[Configuracion::NUMERO_ENTRADAS];
    double *neuronaVecino = new double[Configuracion::NUMERO_ENTRADAS];
    int indiceNeuronaVecina;
    int numero_neurona_lado = 1;
    int total_neuronas_ciclo;
    int cont_num_neurxlado ;

    mapaHex[fila][columna].x = mapaHex[fila][columna].y = 0;

    while (contador_neuronas < Configuracion::NUMERO_NEURONAS) {
        contador_lado = 0;
        total_neuronas_ciclo = 6 * iter;
        numero_neurona_lado = total_neuronas_ciclo/6;
        cont_num_neurxlado = 0;
        for (int ilado = 0; ilado < total_neuronas_ciclo; ilado++) {
            if (cont_num_neurxlado < numero_neurona_lado) {//cuenta el numero de neurona de un lado

                indiceNeuronaVecina = Configuracion::LARGO * fila + columna;
                Arreglos::getNeurona(neuronaOrigen, som->redNeuronal, indiceNeuronaVecina);
                indiceNeuronaVecina = Configuracion::LARGO * fila_v + columna_v;
                Arreglos::getNeurona(neuronaVecino, som->redNeuronal, indiceNeuronaVecina);

                distancia = Distancias::distanciaEuclidea_1(neuronaOrigen, neuronaVecino);

                if (contador_lado + 1 == 1) {//lado1
                    mapaHex[fila_v][columna_v].y = 0;
                    mapaHex[fila_v][columna_v].x = distancia;
                    mapaHex[fila_v][columna_v].lado4.distancia = mapaHex[fila][columna].lado1.distancia = distancia;
                    columna_v = mapaHex[fila][columna].lado2.columna;
                    fila_v = mapaHex[fila][columna].lado2.fila;
                }
                if(contador_lado + 1 == 2){
                    double ladoc = mapaHex[fila][columna].lado1.distancia;
                    double ladob = distancia;
                    int indice_neu_l1 = Configuracion::LARGO * mapaHex[fila_v][columna_v].lado6.fila +
                            mapaHex[columna][fila].lado6.columna;
                    Arreglos::getNeurona(neuronaOrigen, som->redNeuronal, indice_neu_l1);
                    double ladoa = Distancias::distanciaEuclidea_1(neuronaOrigen, neuronaVecino);
                    double p = (ladoa +ladob+ ladoc)/2;
                    double alfa = 2*sqrt(p*(p-ladoa)(p-ladob)(p-ladoc));
                    double h = alfa/ladoc;
                    mapaHex[fila_v][columna_v].y = mapaHex[fila][columna].y - h;
                    mapaHex[fila_v][columna_v].x = mapaHex[fila][columna].x+ sqrt(pow(h, 2) - pow(ladob, 2));
                }
                cont_num_neurxlado++;
            } else {
                if (contador_lado + 1 == 1) {//lado 1
                    columna_v = mapaHex[fila][columna].lado2.columna;
                    fila_v = mapaHex[fila][columna].lado2.fila;
                }
                if (contador_lado +1 == 2){//lado 2
                    columna_v = mapaHex[fila][columna].lado3.columna;
                    fila_v = mapaHex[fila][columna].lado3.fila;
                }
                if (contador_lado +1 == 3){//lado 2
                    columna_v = mapaHex[fila][columna].lado4.columna;
                    fila_v = mapaHex[fila][columna].lado4.fila;
                }
                if (contador_lado +1 == 4){//lado 2
                    columna_v = mapaHex[fila][columna].lado5.columna;
                    fila_v = mapaHex[fila][columna].lado5.fila;
                }
                if (contador_lado +1 == 5){//lado 2
                    columna_v = mapaHex[fila][columna].lado6.columna;
                    fila_v = mapaHex[fila][columna].lado6.fila;
                }
                if (contador_lado +1 == 6){//lado 2
                    columna = mapaHex[fila_v][columna_v].lado1.columna;
                    fila = mapaHex[fila_v][columna_v].lado1.fila;
                    columna_v = mapaHex[fila][columna].lado1.columna;
                    fila_v = mapaHex[fila][columna].lado1.fila;
                }
                contador_lado += 1;
                cont_num_neurxlado=0;
                //...
            }

            contador_neuronas++;
        }
        iter++;
    }
    delete neuronaOrigen;
    delete neuronaVecino;
}

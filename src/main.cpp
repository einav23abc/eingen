#ifndef main_CPP
#define main_CPP

#define FPS 60
#define FRAME_DELAY (1000/FPS)


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Windows.h>
#include "Window.hpp"



int main ( int argc , char *argv[] ) {
    
    Window* window = nullptr;

    //FreeConsole();

    Uint32 frameStart;
    int frameTime;

    window = new Window( "window name" , 100 , 100 , 512 , 320 , 256 , 160 , true, false );


    while ( window->running() ){
        frameStart = SDL_GetTicks();

        window->handleEvents();
        window->update();
        window->render();

        frameTime = SDL_GetTicks() - frameStart;
        //std::cout << "dt:" << frameTime-FRAME_DELAY << "\n";
        if ( FRAME_DELAY > frameTime ){
            SDL_Delay( FRAME_DELAY - frameTime );
        };
    };

    window->clean();
    
    return 0;
};
#endif
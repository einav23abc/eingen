#ifndef App_HPP
#define App_HPP

class App;

#include "Window.hpp"
#include "Room.hpp"
#include "TileLayer.hpp"
#include "Sprite.hpp"
#include "LinkedList.hpp"
#include <math.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <iostream>


const int APPLICATION_SURFACE_WIDTH = 256;
const int APPLICATION_SURFACE_HEIGHT = 160;

typedef struct {
    SDL_Texture* surface;
    int width;
    int height;
} surface_t;

class App {
    public:
        App( Window* parentWindow );
        ~App();
        SDL_Renderer* get_renderer(){ return this->renderer; };
        void Render();
        void Update();
        static void event( void* voidThis , SDL_Event* event ){
            App* _this = (App*)voidThis;
            if ( _this->current_room != nullptr ){
                _this->current_room->event( event );
            };
            return;
        };

        int create_surface( int width , int height );
        void draw_surface ( int surface_id );
        void draw_surface ( int surface_id , SDL_Rect* srcRect , SDL_Rect* destRect );
        void set_rendering_surface ( int surface_id );
        
        int create_room( unsigned int max_cameras_amount , unsigned int max_tile_layers_amount );
        Room* get_room ( unsigned int index );
        void set_current_room( unsigned int index );
        Room* get_current_room();
    private:
        Window* parentWindow;
        SDL_Renderer* renderer;

        SDL_Texture* application_surface;
        int last_created_surface_id = -1;
        surface_t* surfaces = nullptr;
        surface_t current_surface;

        unsigned int rooms_amount;
        Room** rooms;
        unsigned int current_room_index;
        Room* current_room;
};

#endif
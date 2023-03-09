#ifndef App_CPP
#define App_CPP
#include "App.hpp"

#include "Room.cpp"
#include "TileLayer.cpp"
#include "Object.cpp"
#include "Sprite.cpp"




App::App( Window* parentWindow ){
    srand(time(0));

    this->renderer = parentWindow->renderer;
    this->create_surface( APPLICATION_SURFACE_WIDTH , APPLICATION_SURFACE_HEIGHT );
    this->application_surface = this->surfaces[0].surface;

    this->parentWindow = parentWindow;
    this->parentWindow->listenToEvent( (void*)this , (eventFunct_t)&(App::event) );

    this->rooms_amount = 0;
    this->rooms = nullptr;
    this->current_room_index = 0;
    this->current_room = nullptr;

    game_setup( this );

    return;
};

App::~App(){
    return;
};

void App::Render(){
    SDL_SetRenderTarget( this->renderer , this->application_surface );
    SDL_SetRenderDrawColor( this->renderer , 61 , 61 , 61 , 255 );
    SDL_RenderClear( this->renderer );
    
    if ( this->current_room != nullptr ){
        this->current_room->sort_depths();
        this->current_room->draw_camera_view(this->renderer , 0);
    };
    
    // drawing canvas to screen
    int pixelSize = std::min( this->parentWindow->width/APPLICATION_SURFACE_WIDTH , this->parentWindow->height/APPLICATION_SURFACE_HEIGHT );
    int topLeftX = ( this->parentWindow->width - pixelSize*APPLICATION_SURFACE_WIDTH )/2;
    int topLeftY = ( this->parentWindow->height - pixelSize*APPLICATION_SURFACE_HEIGHT )/2;
    SDL_Rect destRect;
    destRect.x = topLeftX;
    destRect.y = topLeftY;
    destRect.w = APPLICATION_SURFACE_WIDTH*pixelSize;
    destRect.h = APPLICATION_SURFACE_HEIGHT*pixelSize;

    SDL_SetRenderTarget( this->renderer , NULL );
    SDL_RenderCopy(this->renderer, this->application_surface, NULL, &destRect);
    return;
};

void App::Update(){
    
    if ( this->current_room != nullptr ){
        this->current_room->step();
    };
    
    return;
};


int App::create_surface( int width , int height ){
    void* tmp = realloc( this->surfaces , sizeof(surface_t)*(this->last_created_surface_id+2) );
    if ( tmp == NULL ){
        std::cout << "Failed to reallocate memory at App::create_surface\n";
        return -1;
    };
    this->last_created_surface_id++;
    this->surfaces = (surface_t*)tmp;
    //SDL_Surface* surface = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    SDL_Texture* surface = SDL_CreateTexture( this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    this->surfaces[this->last_created_surface_id] = surface_t{ surface , width , height };
    return this->last_created_surface_id;
};
void App::draw_surface ( int surface_id ){
    if ( surface_id > this->last_created_surface_id || surface_id < 0 ){
        std::cout << "invalid surface_id at App::draw_surface\n";
        return;
    };
    SDL_Rect srcRect = SDL_Rect{ 0, 0, this->surfaces[surface_id].width, this->surfaces[surface_id].height };
    this->draw_surface( surface_id, &srcRect, NULL );
    return;
};
void App::draw_surface ( int surface_id , SDL_Rect* srcRect , SDL_Rect* destRect ){
    if ( surface_id > this->last_created_surface_id || surface_id < 0 ){
        std::cout << "invalid surface_id at App::draw_surface\n";
        return;
    };
    SDL_RenderCopy( this->renderer, this->surfaces[surface_id].surface, srcRect, destRect);
    return;
};
void App::set_rendering_surface ( int surface_id ){
    if ( surface_id > this->last_created_surface_id || surface_id < 0 ){
        std::cout << "invalid surface_id at App::draw_surface\n";
        return;
    };
    this->current_surface = this->surfaces[surface_id];
    SDL_SetRenderTarget( this->renderer , this->surfaces[surface_id].surface );
    return;
};

int App::create_room( unsigned int max_cameras_amount , unsigned int max_tile_layers_amount ){
    void* tmp = realloc( this->rooms , sizeof(Room*)*(this->rooms_amount+1) );
    if ( tmp == nullptr ){
        std::cout << "realloc failed at App:create_room\n";
        return -1;
    };
    this->rooms = (Room**)tmp;
    Room* _room = new Room( this , max_cameras_amount , max_tile_layers_amount );
    this->rooms[this->rooms_amount] = _room;
    this->rooms_amount++;
    return (this->rooms_amount - 1);
};
Room* App::get_room ( unsigned int index ){
    if ( index >= this->rooms_amount ){
        std::cout << "invalid index at App::get_room\n";
        return nullptr;
    };
    return this->rooms[index];
};
void App::set_current_room( unsigned int index ){
    if ( index >= this->rooms_amount ){
        std::cout << "invalid index at App::set_current_room\n";
        return;
    };
    this->current_room_index = index;
    this->current_room = this->rooms[index];
    return;
};
Room* App::get_current_room(){
    return this->current_room;
};

#endif
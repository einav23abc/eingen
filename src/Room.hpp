#ifndef Room_HPP
#define Room_HPP

class Room;
typedef struct {
    float x;
    float y;
    unsigned int width;
    unsigned int height;
} room_camera_t;

#include "App.hpp"
#include "Object.hpp"
#include "TileLayer.hpp"

class Room {
    public:
        Room( App* parent_app , unsigned int max_cameras_amount , unsigned int max_tile_layers_amount );
        void step();
        void event( SDL_Event* event );
        void sort_depths();
        void draw_simple( SDL_Renderer* renderer );
        void draw_camera_view( SDL_Renderer* renderer , unsigned int camera_index );
        room_camera_t get_camera( unsigned int camera_index );
        void set_camera( unsigned int camera_index , room_camera_t value );
        TileLayer* get_tile_layer( const unsigned int tile_layer_index );
        void set_tile_layer( const unsigned int tile_layer_index , TileLayer* value );
        Object* create_object( ObjectType* type );
        unsigned int get_object_type_amount ( const ObjectType* type );
        Object* get_object_from_type ( const ObjectType* type , unsigned int index );
        Object* get_object_from_id ( const unsigned int id );
    private:
        App* parent_app;
        unsigned int cameras_amount;
        room_camera_t* cameras;
        unsigned int tile_layers_amount;
        TileLayer** tile_layers;
        unsigned int* tile_layers_sorted;
        unsigned int objects_amount;
        unsigned int last_object_id;
        Object** objects;
};

#endif
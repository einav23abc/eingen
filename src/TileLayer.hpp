#ifndef TileLayer_HPP
#define TileLayer_HPP

class TileLayer;

#include "Sprite.hpp"
#include <limits.h>
#include <math.h>

class TileLayer {
    public:
        TileLayer( Sprite* tileset_sprite , const unsigned int max_width , const unsigned int max_height , const float depth );
        void draw_simple( SDL_Renderer* renderer );
        void draw_simple_offseted( SDL_Renderer* renderer , const int offset_x , const int offset_y );
        void draw_section( SDL_Renderer* renderer , const int section_from_pixel_x , const int section_from_pixel_y , const unsigned int section_pixel_width , const unsigned int section_pixel_height );
        void draw_section_offseted( SDL_Renderer* renderer , const int section_from_pixel_x , const int section_from_pixel_y , const unsigned int section_pixel_width , const unsigned int section_pixel_height , const int offset_x , const int offset_y );
        void set_tile( const unsigned int x , const unsigned int y , const unsigned short tile );
        unsigned short get_tile( const unsigned int x , const unsigned int y );
        unsigned short get_tile_at_pixel( const float x , const float y );
        unsigned int get_tile_width(){ return this->sprite_width; };
        unsigned int get_tile_height(){ return this->sprite_height; };
        float get_depth(){ return this->depth; };
    private:
        Sprite* tileset_sprite;
        unsigned int sprite_width;
        unsigned int sprite_height;
        float inverse_sprite_width;
        float inverse_sprite_height;
        int width;
        int height;
        unsigned short* tiles;
        float depth;
};

#endif
#ifndef TileLayer_CPP
#define TileLayer_CPP
#include "TileLayer.hpp"

#include "Sprite.cpp"

TileLayer::TileLayer( Sprite* tileset_sprite , const unsigned int max_width , const unsigned int max_height , const float depth ){
    this->depth = depth;
    this->tileset_sprite = tileset_sprite;
    this->sprite_width = tileset_sprite->getFrameWidth();
    this->sprite_height = tileset_sprite->getFrameHeight();
    this->inverse_sprite_width = 1/(float)this->sprite_width;
    this->inverse_sprite_height = 1/(float)this->sprite_height;
    this->width = max_width;
    this->height = max_height;
    this->tiles = (unsigned short*)calloc( max_width*max_height , sizeof(unsigned short) );
    if( this->tiles == NULL ){
        std::cout << "failed calloc at TileLayer::TileLayer\n";
    };
    return;
};
void TileLayer::draw_simple( SDL_Renderer* renderer ){
    // same as draw_simple_offseted( renderer , 0 , 0 );
    for ( int y = 0; y < this->height; y++ ){
        for ( int x = 0; x < this->width; x++ ){
            this->tileset_sprite->draw( renderer , x*this->sprite_width , y*this->sprite_height , this->tiles[y*this->width+x] );
        };
    };
    return;
};
void TileLayer::draw_simple_offseted( SDL_Renderer* renderer , const int offset_x , const int offset_y ){
    for ( int y = 0; y < this->height; y++ ){
        for ( int x = 0; x < this->width; x++ ){
            this->tileset_sprite->draw( renderer , x*this->sprite_width-offset_x , y*this->sprite_height-offset_y , this->tiles[y*this->width+x] );
        };
    };
    return;
};
void TileLayer::draw_section( SDL_Renderer* renderer , const int section_from_pixel_x , const int section_from_pixel_y , const unsigned int section_pixel_width , const unsigned int section_pixel_height ){
    // same as draw_section_offseted( renderer , section_from_pixel_x , section_from_pixel_y , section_pixel_width , section_pixel_height , 0 , 0 );
    const int from_tile_x = floor(section_from_pixel_x*this->inverse_sprite_width);
    const int from_tile_y = floor(section_from_pixel_y*this->inverse_sprite_height);
    const int tiles_x = floor(section_pixel_width*this->inverse_sprite_width);
    const int tiles_y = floor(section_pixel_height*this->inverse_sprite_height);
    for ( int y = std::max(from_tile_y,0); y < std::min(this->height,from_tile_y+tiles_y+1); y++ ){
        for ( int x = std::max(from_tile_x,0); x < std::min(this->width,from_tile_x+tiles_x+1); x++ ){
            this->tileset_sprite->draw( renderer , x*this->sprite_width-section_from_pixel_x , y*this->sprite_height-section_from_pixel_y , this->tiles[y*this->width+x] );
        };
    };
    return;
};
void TileLayer::draw_section_offseted( SDL_Renderer* renderer , const int section_from_pixel_x , const int section_from_pixel_y , const unsigned int section_pixel_width , const unsigned int section_pixel_height , const int offset_x , const int offset_y ){
    const int from_tile_x = floor(section_from_pixel_x*this->inverse_sprite_width);
    const int from_tile_y = floor(section_from_pixel_y*this->inverse_sprite_height);
    const int tiles_x = ceil(section_pixel_width*this->inverse_sprite_width);
    const int tiles_y = ceil(section_pixel_height*this->inverse_sprite_height);
    for ( int y = std::max(from_tile_y,0); y < std::min(this->height,from_tile_y+tiles_y); y++ ){
        for ( int x = std::max(from_tile_x,0); x < std::min(this->width,from_tile_x+tiles_x); x++ ){
            this->tileset_sprite->draw( renderer , x*this->sprite_width-section_from_pixel_x-offset_x , y*this->sprite_height-section_from_pixel_y-offset_y , this->tiles[y*this->width+x] );
        };
    };
    return;
};

void TileLayer::set_tile( const unsigned int x , const unsigned int y , const unsigned short tile ){
    if ( x < 0 || x >= this->width || y < 0 || y >= this->height ){
        std::cout << "invalid cord{x,y} at TileLayer::set_tile\n";
        return;
    };
    this->tiles[y*this->width+x] = tile;
    return;
};
unsigned short TileLayer::get_tile( const unsigned int x , const unsigned int y ){
    if ( x < 0 || x >= this->width || y < 0 || y >= this->height ){
        std::cout << "invalid cord{x,y} at TileLayer::get_tile\n";
        return 0;
    };
    return this->tiles[y*this->width+x];
};
unsigned short TileLayer::get_tile_at_pixel( const float x , const float y ){
    return this->get_tile( x*this->inverse_sprite_width , y*this->inverse_sprite_height );
};

#endif
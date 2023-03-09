#ifndef Sprite_HPP
#define Sprite_HPP

#include <cmath>

class Sprite {
    public:
        Sprite( const char* textureFileName , const int _frameWidth , const int _frameHeight , SDL_Renderer* renderer );
        Sprite( const char* textureFileName , const int _frameWidth , const int _frameHeight , SDL_Renderer* renderer , int draw_offset_x , int draw_offset_y );
        ~Sprite();
        void draw ( SDL_Renderer* renderer , const int x , const int y , const int frame );
        int getFrameWidth(){ return this->frameWidth; };
        int getFrameHeight(){ return this->frameHeight; };
        int getFrames(){ return this->frames; };
        SDL_Texture* texture;
    private:
        int textureWidth;
        int textureheight;
        unsigned int frameWidth;
        unsigned int frameHeight;
        unsigned int framesPerRow;
        unsigned int frameRows;
        unsigned int frames;
        int draw_offset_x;
        int draw_offset_y;
};

#endif
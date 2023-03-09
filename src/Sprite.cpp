#ifndef Sprite_CPP
#define Sprite_CPP
#include "Sprite.hpp"

Sprite::Sprite( const char* textureFileName , const int _frameWidth , const int _frameHeight , SDL_Renderer* renderer ){
    this->draw_offset_x = 0;
    this->draw_offset_y = 0;
    
    SDL_Surface* tempSurface = IMG_Load( textureFileName );
    this->texture = SDL_CreateTextureFromSurface( renderer , tempSurface );
    SDL_FreeSurface( tempSurface );

    SDL_QueryTexture(this->texture, NULL, NULL, &this->textureWidth, &this->textureheight);
    this->frameWidth = _frameWidth;
    this->frameHeight = _frameHeight;

    this->framesPerRow = this->textureWidth/_frameWidth;
    this->frameRows = this->textureheight/_frameHeight;
    this->frames = this->frameRows*this->framesPerRow;
    return;
};
Sprite::Sprite( const char* textureFileName , const int _frameWidth , const int _frameHeight , SDL_Renderer* renderer , int draw_offset_x , int draw_offset_y ){
    this->draw_offset_x = draw_offset_x;
    this->draw_offset_y = draw_offset_y;
    
    SDL_Surface* tempSurface = IMG_Load( textureFileName );
    this->texture = SDL_CreateTextureFromSurface( renderer , tempSurface );
    SDL_FreeSurface( tempSurface );

    SDL_QueryTexture(this->texture, NULL, NULL, &this->textureWidth, &this->textureheight);
    this->frameWidth = _frameWidth;
    this->frameHeight = _frameHeight;

    this->framesPerRow = this->textureWidth/_frameWidth;
    this->frameRows = this->textureheight/_frameHeight;
    this->frames = this->frameRows*this->framesPerRow;
};

Sprite::~Sprite(){
    return;
};


void Sprite::draw ( SDL_Renderer* renderer , const int x , const int y , const int frame ){
    int _frame = frame%this->frames;
    SDL_Rect srcRect, destRect;
    destRect.x = x-this->draw_offset_x;
    destRect.y = y-this->draw_offset_y;
    destRect.w = this->frameWidth;
    destRect.h = this->frameHeight;
    srcRect.x = ((_frame)%this->framesPerRow)*this->frameWidth;
    srcRect.y = ((int)(_frame/this->framesPerRow))*this->frameHeight;
    srcRect.w = this->frameWidth;
    srcRect.h = this->frameHeight;
    SDL_RenderCopy( renderer , this->texture , &srcRect , &destRect );
    return;
};

#endif
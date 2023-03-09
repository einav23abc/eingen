#ifndef Object_HPP
#define Object_HPP

class ObjectType;
class Object;
typedef void(*ObjectScript_t)( Object* );
typedef void(*ObjectDrawScript_t)( Object* , SDL_Renderer* , const int , const int );
typedef void(*ObjectEventScript_t)( Object* , SDL_Event* );
void ObjectScript_deafult( Object* _this );
void ObjectDrawScript_deafult( Object* _this , SDL_Renderer* , const int , const int );
void ObjectEventScript_deafult( Object* _this , SDL_Event* event );

#include "Room.hpp"
#include "Sprite.hpp"
#include "App.hpp"

class ObjectType {
    public:
        ObjectType( ObjectScript_t create , ObjectScript_t step , ObjectDrawScript_t draw , ObjectEventScript_t event , size_t variables_size );
        ObjectScript_t get_script_create(){ return this->script_create; };
        ObjectScript_t get_script_step(){ return this->script_step; };
        ObjectDrawScript_t get_script_draw(){ return this->script_draw; };
        ObjectEventScript_t get_script_event(){ return this->script_event; };
        size_t get_variables_size(){ return this->variables_size; };
    private:
        ObjectScript_t script_create;
        ObjectScript_t script_step;
        ObjectDrawScript_t script_draw;
        ObjectEventScript_t script_event;
        size_t variables_size;
};

class Object {
    public:
        Object( App* parent_app , Room* parent_room , ObjectType* type , unsigned int id ); // create
        Object( App* parent_app , Room* parent_room , Object* object_to_duplicate , unsigned int id ); // duplicate another object
        void step ();
        void draw_simple( SDL_Renderer* renderer );
        void draw_simple_offseted( SDL_Renderer* renderer , const int offset_x , const int offset_y );
        void event ( SDL_Event* event );

        unsigned int get_id(){return this->id;};
        App* get_parent_app(){return this->parent_app;};
        Room* get_parent_room(){return this->parent_room;};
        ObjectType* get_object_type(){return this->type;};
        Sprite* sprite;
        unsigned int frame;
        float x;
        float y;
        float depth;
        void* variables;
    private:
        unsigned int id;
        ObjectType* type;
        App* parent_app;
        Room* parent_room;
};

#endif
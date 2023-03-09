#ifndef Object_CPP
#define Object_CPP
#include "Object.hpp"

#include "Room.cpp"
#include "Sprite.cpp"
#include "App.cpp"

void ObjectScript_deafult( Object* _this ){return;};
void ObjectDrawScript_deafult( Object* _this , SDL_Renderer* renderer , const int offset_x , const int offset_y ){
    if ( _this->sprite == nullptr ){
        return;
    };
    _this->sprite->draw( renderer , _this->x-offset_x , _this->y-offset_y , _this->frame );
    return;
};
void ObjectEventScript_deafult( Object* _this , SDL_Event* event ){return;};


ObjectType::ObjectType( ObjectScript_t create , ObjectScript_t step , ObjectDrawScript_t draw , ObjectEventScript_t event , size_t variables_size ){
    this->variables_size = variables_size;
    if ( create == nullptr ){
        this->script_create = ObjectScript_deafult;
    }else{
        this->script_create = create;
    };
    if ( draw == nullptr ){
        this->script_draw = ObjectDrawScript_deafult;
    }else{
        this->script_draw = draw;
    };
    if ( step == nullptr ){
        this->script_step = ObjectScript_deafult;
    }else{
        this->script_step = step;
    };
    if ( event == nullptr ){
        this->script_event = ObjectEventScript_deafult;
    }else{
        this->script_event = event;
    };
    return;
};



Object::Object ( App* parent_app , Room* parent_room , ObjectType* type , unsigned int id ){
    this->id = id;
    this->parent_room = parent_room;
    this->parent_app = parent_app;
    this->type = type;
    this->variables = calloc( 1 , type->get_variables_size() );
    if ( this->variables == nullptr ){
        std::cout << "calloc failed at Object::Object(App*,Room*,ObjectType*,unsigned int)\n";
    };
    this->type->get_script_create()(this);
    return;
};
Object::Object( App* parent_app , Room* parent_room , Object* object_to_duplicate , unsigned int id ){
    this->id = id;
    this->parent_room = parent_room;
    this->parent_app = parent_app;
    this->type = object_to_duplicate->get_object_type();
    this->sprite = object_to_duplicate->sprite;
    this->frame = object_to_duplicate->frame;
    this->x = object_to_duplicate->x;
    this->y = object_to_duplicate->y;
    this->depth = object_to_duplicate->depth;
    this->variables = calloc( 1 , type->get_variables_size() );
    if ( this->variables == nullptr ){
        std::cout << "calloc failed at Object::Object(App*,Room*,Object*,unsigned int)\n";
    };
    memcpy( this->variables , object_to_duplicate->variables , type->get_variables_size() );
    return;
};
void Object::step (){
    this->type->get_script_step()(this);
    return;
};
void Object::draw_simple( SDL_Renderer* renderer ){
    this->draw_simple_offseted(renderer,0,0);
    return;
};
void Object::draw_simple_offseted( SDL_Renderer* renderer , const int offset_x , const int offset_y ){
    this->type->get_script_draw()(this,renderer,offset_x,offset_y);
    return;
};
void Object::event ( SDL_Event* event ){
    this->type->get_script_event()(this,event);
    return;
};


#endif
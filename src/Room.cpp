#ifndef Room_CPP
#define Room_CPP
#include "Room.hpp"

#include "App.cpp"
#include "Object.cpp"
#include "TileLayer.cpp"

Room::Room( App* parent_app , unsigned int max_cameras_amount , unsigned int max_tile_layers_amount ){
    this->objects = nullptr;
    this->objects_amount = 0;
    this->last_object_id = 0;
    this->parent_app = parent_app;
    this->cameras_amount = max_cameras_amount;
    this->tile_layers_amount = max_tile_layers_amount;
    this->cameras = (room_camera_t*)calloc( max_cameras_amount , sizeof(room_camera_t) );
    this->tile_layers = (TileLayer**)calloc( max_tile_layers_amount , sizeof(TileLayer*) );
    this->tile_layers_sorted = (unsigned int*)calloc( max_tile_layers_amount , sizeof(unsigned int) );
    if ( this->cameras == nullptr || this->tile_layers == nullptr || this->tile_layers_sorted == nullptr ){
        std::cout << "faild calloc at Room:Room\n";
        return;
    };
    for ( int i = 0; i < max_tile_layers_amount; i++ ){
        this->tile_layers_sorted[i] = i;
    };
    return;
};
void Room::step(){
    for( int o = 0; o < this->objects_amount; o++ ){
        this->objects[o]->step();
    };
    return;
};
void Room::event( SDL_Event* event ){
    for( int o = 0; o < this->objects_amount; o++ ){
        this->objects[o]->event(event);
    };
    return;
};
void Room::sort_depths(){
    // <sort objects>

        // // print current depths
        // std::cout << "\n\nobject depths: [ ";
        // for ( unsigned int o = 0; o < this->objects_amount; o++ ){
        //     if ( o != 0 ){ std::cout << ", "; };
        //     std::cout << this->objects[o]->depth << " ";
        // };
        // std::cout << "]\n";

        // <turn to max-heap tree>
            int i = this->objects_amount;
            i = i - (i%2);
            i--;
            while( i > 0 ){
                const unsigned int heap_par_obj = (i-1)*0.5;
                unsigned int i_smaller;
                if ( i+1 >= this->objects_amount || this->objects[i]->depth < this->objects[i+1]->depth ){
                    i_smaller = i;
                }else{
                    i_smaller = i+1;
                };
                if ( this->objects[i_smaller]->depth < this->objects[heap_par_obj]->depth ){
                    // std::cout << "switch:" << "{i:" << i_smaller << ",d:" << this->objects[i_smaller]->depth << "} <-> {" << "{i:" << heap_par_obj << ",d:" << this->objects[heap_par_obj]->depth << "}\n";
                    Object* tmp = this->objects[i_smaller];
                    this->objects[i_smaller] = this->objects[heap_par_obj];
                    this->objects[heap_par_obj] = tmp;

                    unsigned int j_heap_par_obj = i_smaller;
                    unsigned int j = i_smaller*2+1;
                    while(true){
                        unsigned int j_smaller;
                        if ( j >= this->objects_amount ){ break; };
                        if ( j+1 >= this->objects_amount || this->objects[j]->depth < this->objects[j+1]->depth ){
                            j_smaller = j;
                        }else{
                            j_smaller = j+1;
                        };
                        if ( this->objects[j_smaller]->depth >= this->objects[j_heap_par_obj]->depth ){
                            break;
                        };
                        // std::cout << "_switch_:" << "{i:" << j_smaller << ",d:" << this->objects[j_smaller]->depth << "} <-> {" << "{i:" << j_heap_par_obj << ",d:" << this->objects[j_heap_par_obj]->depth << "}\n";
                        Object* tmp = this->objects[j_smaller];
                        this->objects[j_smaller] = this->objects[j_heap_par_obj];
                        this->objects[j_heap_par_obj] = tmp;
                        j_heap_par_obj = j_smaller;
                        j = j_smaller*2+1;
                    };
                };
                i -= 2;
            };
        // </turn to max-heap tree>

        // <sort>
            unsigned int unsorted_size = this->objects_amount;
            while( unsorted_size > 1 ){
                // // print current depths
                // std::cout << "object depths: [ ";
                // for ( unsigned int o = 0; o < this->objects_amount; o++ ){
                //     if ( o != 0 ){ std::cout << ", "; };
                //     std::cout << this->objects[o]->depth << " ";
                // };
                // std::cout << "]\n";

                // switch first and last
                // std::cout << "perm_switch:" << "{i:" << 0 << ",d:" << this->objects[0]->depth << "} <-> {" << "{i:" << unsorted_size-1 << ",d:" << this->objects[unsorted_size-1]->depth << "}\n";
                Object* tmp = this->objects[0];
                this->objects[0] = this->objects[unsorted_size-1];
                this->objects[unsorted_size-1] = tmp;
                unsorted_size--;
                // fix max-heap tree
                unsigned int j_heap_par_obj = 0;
                unsigned int j = 1;
                while(true){
                    unsigned int j_smaller;
                    if ( j >= unsorted_size ){ break; };
                    if ( j+1 >= unsorted_size || this->objects[j]->depth < this->objects[j+1]->depth ){
                        j_smaller = j;
                    }else{
                        j_smaller = j+1;
                    };
                    if ( this->objects[j_smaller]->depth >= this->objects[j_heap_par_obj]->depth ){
                        break;
                    };
                    // std::cout << "_switch_:" << "{i:" << j_smaller << ",d:" << this->objects[j_smaller]->depth << "} <-> {" << "{i:" << j_heap_par_obj << ",d:" << this->objects[j_heap_par_obj]->depth << "}\n";
                    Object* tmp = this->objects[j_smaller];
                    this->objects[j_smaller] = this->objects[j_heap_par_obj];
                    this->objects[j_heap_par_obj] = tmp;
                    j_heap_par_obj = j_smaller;
                    j = j_smaller*2+1;
                };
            };
        // </sort>
        
        // // print current depths
        // std::cout << "object depths: [ ";
        // for ( unsigned int o = 0; o < this->objects_amount; o++ ){
        //     if ( o != 0 ){ std::cout << ", "; };
        //     std::cout << this->objects[o]->depth << " ";
        // };
        // std::cout << "]\n";

    // </sort objects>
    return;
};
void Room::draw_simple( SDL_Renderer* renderer ){
    unsigned int tl = 0;
    unsigned int o = 0;
    while ( ( tl < this->tile_layers_amount && this->tile_layers[tl] != nullptr ) && o < this->objects_amount ){
        if ( this->objects[o]->depth > this->tile_layers[tl]->get_depth() ){
            this->objects[o]->draw_simple( renderer );
            o++;
        }else{
            this->tile_layers[tl]->draw_simple( renderer );
            tl++;
        };
    };
    while ( ( tl < this->tile_layers_amount && this->tile_layers[tl] != nullptr ) ){
        this->tile_layers[tl]->draw_simple( renderer );
        tl++;
    };
    while ( o < this->objects_amount ){
        this->objects[o]->draw_simple( renderer );
        o++;
    };
    return;
};
void Room::draw_camera_view( SDL_Renderer* renderer , unsigned int camera_index ){
    //std::cout << "Room::draw_camera_view(" << renderer << "," << camera_index << ")\n";
    if ( camera_index >= this->cameras_amount ){
        std::cout << "invalid camera_index at Room:draw_camera_view\n";
        return;
    };
    const room_camera_t _cam = this->cameras[camera_index];
    
    unsigned int tl = 0;
    unsigned int o = 0;
    while ( ( tl < this->tile_layers_amount && this->tile_layers[tl] != nullptr ) && o < this->objects_amount ){
        if ( this->objects[o]->depth > this->tile_layers[tl]->get_depth() ){
            this->objects[o]->draw_simple_offseted( renderer , (int)_cam.x , (int)_cam.y );
            o++;
        }else{
            //std::cout << "drawing layer " << tl << "\n";
            this->tile_layers[tl]->draw_section( renderer , (int)_cam.x , (int)_cam.y , _cam.width , _cam.height );
            tl++;
        };
    };
    while ( tl < this->tile_layers_amount && this->tile_layers[tl] != nullptr ){
        //std::cout << "drawing layer " << tl << "\n";
        this->tile_layers[tl]->draw_section( renderer , (int)_cam.x , (int)_cam.y , _cam.width , _cam.height );
        tl++;
    };
    while ( o < this->objects_amount ){
        this->objects[o]->draw_simple_offseted( renderer , (int)_cam.x , (int)_cam.y );
        o++;
    };
    return;
};
room_camera_t Room::get_camera( unsigned int camera_index ){
    if ( camera_index >= this->cameras_amount ){
        std::cout << "invalid camera_index at Room:get_camera\n";
        return room_camera_t{0,0,0,0};
    };
    return this->cameras[camera_index];
};
void Room::set_camera( unsigned int camera_index , room_camera_t value ){
    if ( camera_index >= this->cameras_amount ){
        std::cout << "invalid camera_index at Room:set_camera\n";
        return;
    };
    this->cameras[camera_index] = value;
    return;
};
TileLayer* Room::get_tile_layer( const unsigned int tile_layer_index ){
    if ( tile_layer_index >= this->tile_layers_amount ){
        std::cout << "invalid camera_index at Room:get_tile_layer\n";
        return nullptr;
    };
    return this->tile_layers[tile_layer_index];
};
void Room::set_tile_layer( const unsigned int tile_layer_index , TileLayer* value ){
    if ( tile_layer_index >= this->tile_layers_amount ){
        std::cout << "invalid camera_index at Room:set_tile_layer\n";
        return;
    };
    this->tile_layers[tile_layer_index] = value;
    // <sort layers>
        // find where the layer is on the sorted array
        unsigned int layer_sorted_index = 0;
        while(true){
            if ( this->tile_layers_sorted[layer_sorted_index] == tile_layer_index ){
                break;
            };
            layer_sorted_index++;
        };
        // put the layer at pos 0 (shifting evrything to not change order)
        for ( unsigned int i = layer_sorted_index; i > 0; i-- ){
            this->tile_layers_sorted[i] = this->tile_layers_sorted[i-1];
        };
        this->tile_layers_sorted[0] = tile_layer_index;
        // shift the layer until at correct pos in sorted array
        unsigned int i = 0;
        while( true ){
            if ( i+1 >= this->tile_layers_amount ){
                break;
            };
            if ( this->tile_layers[this->tile_layers_sorted[i+1]] == nullptr ){
                break;
            };
            if ( this->tile_layers[this->tile_layers_sorted[i+1]]->get_depth() < this->tile_layers[this->tile_layers_sorted[i]]->get_depth() ){
                break;
            };
            unsigned int tmp = this->tile_layers_sorted[i+1];
            this->tile_layers_sorted[i+1] = this->tile_layers_sorted[i];
            this->tile_layers_sorted[i] = tmp;
            i++;
        };
        // // print current depths
        // std::cout << "layer depths: [ ";
        // for ( unsigned int tl = 0; tl < this->tile_layers_amount; tl++ ){
        //     if ( tl != 0 ){ std::cout << ", "; };
        //     if ( this->tile_layers[this->tile_layers_sorted[tl]] == nullptr ){
        //         std::cout << "nullptr ";
        //     }else{
        //         std::cout << this->tile_layers[this->tile_layers_sorted[tl]]->get_depth() << " ";
        //     };
        // };
        // std::cout << "]\n";
    // </sort layers>
    return;
};
Object* Room::create_object( ObjectType* type ){
    void* tmp = realloc( this->objects , sizeof(Object*)*(this->objects_amount+1) );
    if ( tmp == nullptr ){
        std::cout << "realloc failed at Room:create_object\n";
        return nullptr;
    };
    this->objects = (Object**)tmp;
    Object* _object = new Object( this->parent_app , this , type , this->last_object_id );
    this->objects[objects_amount] = _object;
    this->objects_amount++;
    this->last_object_id++;
    return _object;
};
unsigned int Room::get_object_type_amount ( const ObjectType* type ){
    unsigned int amount = 0;
    for ( unsigned int o = 0; o < this->objects_amount; o++ ){
        if ( this->objects[o]->get_object_type() == type ){
            amount++;
        };
    };
    return amount;
};
Object* Room::get_object_from_type ( const ObjectType* type , unsigned int index ){
    for ( unsigned int o = 0; o < this->objects_amount; o++ ){
        if ( this->objects[o]->get_object_type() == type ){
            if ( index == 0 ){
                return this->objects[o];
            };
            index--;
        };
    };
    return nullptr;
};
Object* Room::get_object_from_id ( const unsigned int id ){
    for ( unsigned int o = 0; o < this->objects_amount; o++ ){
        if ( this->objects[o]->get_id() == id ){
            return this->objects[o];
        };
    };
    return nullptr;
};

#endif
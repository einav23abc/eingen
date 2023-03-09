#include "../src/main.cpp"
#ifndef game_1_CPP
#define game_1_CPP

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//               ____      ____ ____           ____ ____ ____           ____ ____           ____                ____ ____ ____      ____ ____                ____           ____ ____ ____ ____ ____      ____ ____ ____           ____ ____           ____                ____      ____             //
//             /|    |   /|    |    |        /|    |    |    |        /|    |    |        /|    |             /|    |    |    |   /|    |    |             /|    |        /|    |    |    |    |    |   /|    |    |    |        /|    |    |        /|    |             /|    |   /|    |            //
//          __|_|____|  | |____|____|____   | |____|____|____|     __|_|____|____|____   | |____|            | |____|____|____|  | |____|____|____      __|_|____|____   | |____|____|____|____|____|  | |____|____|____|     __|_|____|____|____   | |____|____        | |____|  | |____|____        //
//        /|    |___/   |/|    |___/|    |  |/|    |___/____/    /|    |___/____/|    |  |/|    |            |/|    |___/____/   |/|    |___/|    |   /|    |___/|    |  |/____/____/|    |___/____/   |/____/|    |___/    /|    |___/____/|    |  |/|    |    |       |/|    |  |/____/|    |       //
//     __|_|____|       | |____|  | |____|  | |____|____        | |____|       | |____|  | |____|            | |____|____        | |____|__|_|____|  | |____|__|_|____|            | |____|                 | |____|       | |____|       | |____|  | |____|____|____   | |____|       | |____|____   //
//   /|    |___/        |/|    |  |/|    |  |/|    |    |       |/|    |       |/____/   |/|    |            |/|    |    |       |/|    |    |___/   |/|    |    |    |            |/|    |                 |/|    |       |/|    |       |/|    |  |/|    |___/|    |  |/|    |       |/____/|    |  //
//  | |____|____        | |____|  | |____|  | |____|____|       | |____|          ____   | |____|            | |____|____|       | |____|____|____   | |____|____|____|            | |____|                 | |____|       | |____|       | |____|  | |____|  | |____|__|_|____|          __|_|____|  //
//  |/____/|    |       |/|    |  |/|    |  |/|    |___/        |/|    |        /|    |  |/|    |            |/|    |___/        |/|    |___/|    |  |/|    |___/|    |            |/|    |                 |/|    |       |/|    |       |/|    |  |/|    |  |/____/|    |    |        /|    |___/   //
//       | |____|____   | |____|__|_|____|  | |____|____ ____   | |____|____ __|_|____|  | |____|____ ____   | |____|____ ____   | |____|  | |____|  | |____|  | |____|            | |____|               __|_|____|____   | |____|____ __|_|____|  | |____|       | |____|____|     __|_|____|       //
//       |/____/|    |  |/|    |    |___/   |/|    |    |    |  |/____/|    |    |___/   |/|    |    |    |  |/|    |    |    |  |/|    |  | |    |  |/|    |  | |    |            |/|    |             /|    |    |    |  |/____/|    |    |___/   |/|    |       |/____/|    |   /|    |___/        //
//            | |____|  | |____|____|       | |____|____|____|       | |____|____|       | |____|____|____|  | |____|____|____|  | |____|  | |____|  | |____|  | |____|            | |____|            | |____|____|____|       | |____|____|       | |____|            | |____|  | |____|            //
//            |/____/   |/____/____/        |/____/____/____/        |/____/____/        |/____/____/____/   |/____/____/____/   |/____/   |/____/   |/____/   |/____/             |/____/             |/____/____/____/        |/____/____/        |/____/             |/____/   |/____/             //
//                                                                                                                                                                                                                                                                                                    //
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

Sprite* player_idle_right_sprite;
Sprite* player_idle_left_sprite;
Sprite* player_run_right_sprite;
Sprite* player_run_left_sprite;
ObjectType* player_object_type;
typedef struct {
    float local_frame;

    bool key_left;
    bool key_right;
    bool key_up;

    float vx;
    float vy;

    int last_on_ground_y;
    unsigned int on_ground_buffer;
    unsigned int jump_request_buffer;
} player_variables_t;
const unsigned int PLAYER_on_ground_buffer_length = 10;
const unsigned int PLAYER_jump_request_buffer_length = 5;
const float PLAYER_gravity = 0.38;
const float PLAYER_intended_jump_velocity = -5;
// the actual jump velocity is calculated in runtime so that every jump (even
// ones on coyote time after you started falling) will have the same jump height.
//
// formula found like this:
// using: v = v0 + a*t , y = y0 + v0*t + (a*t*t)/2
// finding t where jump height is at maximum:
// 0 = v0 + a*t
// t = (-v0)/a
const float PLAYER_jump_velocity_formula_iT = ((float)1)/( (-PLAYER_intended_jump_velocity)/PLAYER_gravity );
// finding jump height:
// y = 0 + v0*t + (a*t*t)/2     // moving { (a*t*t)/2 } to other side for efficency
// y - (a*t*t)/2 = v0*t         // replacing { y - (a*t*t)/2 } with { w }
// w = v0*t
const float PLAYER_jump_velocity_formula_W = PLAYER_intended_jump_velocity/PLAYER_jump_velocity_formula_iT;
// final formula:
// w = y0 + v0*t
// w - y0 = v0*t                // 1/t = it
// w - y0 = v0/it
// v0 = ( w - y0 )*it           // < this is the formula ; 'w' and 'it' are known parameters while 'y0' is the diffrence between the current y and the last_on_ground_y



void player_create( Object* _this ){
    _this->sprite = player_idle_right_sprite;
    _this->frame = 0;
    _this->x = rand()%(256-32-16)+16;
    _this->y = rand()%(160-32-24)+16;
    _this->depth = rand();
    
    player_variables_t* _variables = (player_variables_t*)_this->variables;
    _variables->local_frame = 0;
    _variables->key_left = false;
    _variables->key_right = false;
    _variables->key_up = false;
    _variables->vx = 0;
    _variables->vy = 0;
    _variables->last_on_ground_y = 0;
    _variables->on_ground_buffer = 0;
    _variables->jump_request_buffer = 0;
    return;
};
void player_step( Object* _this ){
    player_variables_t* _variables = (player_variables_t*)_this->variables;
    
    Sprite* last_sprite = _this->sprite;

    // jump_request_buffer
    if ( _variables->key_up ){
        _variables->jump_request_buffer = PLAYER_jump_request_buffer_length;
    }else if ( _variables->jump_request_buffer != 0 ){
        _variables->jump_request_buffer--;
    };
    // on_ground_buffer
    if ( _variables->on_ground_buffer != 0 ){
        _variables->on_ground_buffer--;
    };

    // set sprite to idle
    if ( _variables->vx < 0 ){
        _this->sprite = player_idle_left_sprite;
    }else if ( _variables->vx > 0 ){
        _this->sprite = player_idle_right_sprite;
    };

    // reset x movement
    _variables->vx = 0;
    if (
        // move left condition
        _variables->key_left &&
        !_variables->key_right
    ){
        // move left
        _variables->vx = -1.5;
        _this->sprite = player_run_left_sprite;
    }else if (
        // move right condition
        _variables->key_right &&
        !_variables->key_left
    ){
        // move right
        _variables->vx = 1.5;
        _this->sprite = player_run_right_sprite;
    };

    // add gravity
    _variables->vy += PLAYER_gravity;
    if (
        // jump conditions
        _variables->on_ground_buffer > 0 &&
        _variables->jump_request_buffer > 0
    ){
        // jump
        _variables->vy = ( PLAYER_jump_velocity_formula_W - (_this->y-_variables->last_on_ground_y) )*PLAYER_jump_velocity_formula_iT;      // using this formula so that every jump will have the same height relative to the last_on_ground_y and will take the same amount of time to get to that height ; formula explained after player_variables_t decleration
        _variables->on_ground_buffer = 0;
    };
    
    TileLayer* col_tiles = _this->get_parent_room()->get_tile_layer(0);

    _this->x += _variables->vx;
    if ( _variables->vx > 0 ){
        if ( col_tiles->get_tile_at_pixel(_this->x+15.99,_this->y+23.99) != 0 || col_tiles->get_tile_at_pixel(_this->x+15.99,_this->y+12) != 0 || col_tiles->get_tile_at_pixel(_this->x+15.99,_this->y) != 0 ){
            _this->x = (int)_this->x - ((int)_this->x%col_tiles->get_tile_width());
        };
    }else{
        if ( col_tiles->get_tile_at_pixel(_this->x,_this->y+23.99) != 0 || col_tiles->get_tile_at_pixel(_this->x,_this->y+12) != 0 || col_tiles->get_tile_at_pixel(_this->x,_this->y) != 0 ){
            _this->x = (int)_this->x - ((int)_this->x%col_tiles->get_tile_width()) + col_tiles->get_tile_width();
        };
    };

    _this->y += _variables->vy;
    if ( _variables->vy > 0 ){
        if ( col_tiles->get_tile_at_pixel(_this->x,_this->y+23.99) != 0 || col_tiles->get_tile_at_pixel(_this->x+15.99,_this->y+23.99) != 0 ){
            _this->y = (int)_this->y - ((int)(_this->y+24)%col_tiles->get_tile_height());
            _variables->on_ground_buffer = PLAYER_on_ground_buffer_length;
            _variables->last_on_ground_y = _this->y;
            _variables->vy = 0;
        };
    }else{
        if ( col_tiles->get_tile_at_pixel(_this->x,_this->y) != 0 || col_tiles->get_tile_at_pixel(_this->x+15.99,_this->y) != 0 ){
            _this->y = (int)_this->y - ((int)_this->y%col_tiles->get_tile_height()) + col_tiles->get_tile_height();
            _variables->vy = 0;
        };
    };

    _variables->local_frame += ((float)1000/94)/FPS;
    _this->frame = (unsigned int)_variables->local_frame;
    if ( last_sprite != _this->sprite ){
        _this->frame = 0;
        _variables->local_frame = 0;
    };
    

    room_camera_t _room_cam = _this->get_parent_room()->get_camera(0);
    _room_cam.x = (int)((int)_this->x+8-(_room_cam.width/2));
    _this->get_parent_room()->set_camera(0,_room_cam);
    return;
};
void player_event( Object* _this , SDL_Event* event ){
    if ( event->type == SDL_KEYDOWN ){
        const char* keyName = SDL_GetKeyName( event->key.keysym.sym );
        if ( strcmp( keyName , "Left" ) == 0 ){
            ((player_variables_t*)_this->variables)->key_left = true;
        }else if ( strcmp( keyName , "Right" ) == 0 ){
            ((player_variables_t*)_this->variables)->key_right = true;
        }else if ( strcmp( keyName , "Up" ) == 0 ){
            ((player_variables_t*)_this->variables)->key_up = true;
        };
    }else if ( event->type == SDL_KEYUP ){
        const char* keyName = SDL_GetKeyName( event->key.keysym.sym );
        if ( strcmp( keyName , "Left" ) == 0 ){
            ((player_variables_t*)_this->variables)->key_left = false;
        }else if ( strcmp( keyName , "Right" ) == 0 ){
            ((player_variables_t*)_this->variables)->key_right = false;
        }else if ( strcmp( keyName , "Up" ) == 0 ){
            ((player_variables_t*)_this->variables)->key_up = false;
        };
    };
    return;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//               ____                          ____      ____ ____           ____ ____ ____           ____ ____           ____                ____ ____ ____      ____ ____                ____           ____ ____ ____ ____ ____      ____ ____ ____           ____ ____           ____                ____      ____             //
//             /|    |                       /|    |   /|    |    |        /|    |    |    |        /|    |    |        /|    |             /|    |    |    |   /|    |    |             /|    |        /|    |    |    |    |    |   /|    |    |    |        /|    |    |        /|    |             /|    |   /|    |            //
//          __|_|____|                    __|_|____|  | |____|____|____   | |____|____|____|     __|_|____|____|____   | |____|            | |____|____|____|  | |____|____|____      __|_|____|____   | |____|____|____|____|____|  | |____|____|____|     __|_|____|____|____   | |____|____        | |____|  | |____|____        //
//        /|    |___/                   /|    |___/   |/|    |___/|    |  |/|    |___/____/    /|    |___/____/|    |  |/|    |            |/|    |___/____/   |/|    |___/|    |   /|    |___/|    |  |/____/____/|    |___/____/   |/____/|    |___/    /|    |___/____/|    |  |/|    |    |       |/|    |  |/____/|    |       //
//     __|_|____|                    __|_|____|       | |____|  | |____|  | |____|____        | |____|       | |____|  | |____|            | |____|____        | |____|__|_|____|  | |____|__|_|____|            | |____|                 | |____|       | |____|       | |____|  | |____|____|____   | |____|       | |____|____   //
//   /|    |___/                   /|    |___/        |/|    |  |/|    |  |/|    |    |       |/|    |       |/____/   |/|    |            |/|    |    |       |/|    |    |___/   |/|    |    |    |            |/|    |                 |/|    |       |/|    |       |/|    |  |/|    |___/|    |  |/|    |       |/____/|    |  //
//  | |____|____                __|_|____|            | |____|  | |____|  | |____|____|       | |____|          ____   | |____|            | |____|____|       | |____|____|____   | |____|____|____|            | |____|                 | |____|       | |____|       | |____|  | |____|  | |____|__|_|____|          __|_|____|  //
//  |/____/|    |             /|    |___/             |/|    |  |/|    |  |/|    |___/        |/|    |        /|    |  |/|    |            |/|    |___/        |/|    |___/|    |  |/|    |___/|    |            |/|    |                 |/|    |       |/|    |       |/|    |  |/|    |  |/____/|    |    |        /|    |___/   //
//       | |____|____      __|_|____|                 | |____|__|_|____|  | |____|____ ____   | |____|____ __|_|____|  | |____|____ ____   | |____|____ ____   | |____|  | |____|  | |____|  | |____|            | |____|               __|_|____|____   | |____|____ __|_|____|  | |____|       | |____|____|     __|_|____|       //
//       |/____/|    |   /|    |___/                  |/|    |    |___/   |/|    |    |    |  |/____/|    |    |___/   |/|    |    |    |  |/|    |    |    |  |/|    |  | |    |  |/|    |  | |    |            |/|    |             /|    |    |    |  |/____/|    |    |___/   |/|    |       |/____/|    |   /|    |___/        //
//            | |____|  | |____|                      | |____|____|       | |____|____|____|       | |____|____|       | |____|____|____|  | |____|____|____|  | |____|  | |____|  | |____|  | |____|            | |____|            | |____|____|____|       | |____|____|       | |____|            | |____|  | |____|            //
//            |/____/   |/____/                       |/____/____/        |/____/____/____/        |/____/____/        |/____/____/____/   |/____/____/____/   |/____/   |/____/   |/____/   |/____/             |/____/             |/____/____/____/        |/____/____/        |/____/             |/____/   |/____/             //
//                                                                                                                                                                                                                                                                                                                                  //
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


void game_setup( App* parent_app ){

    //-------------------------------------------------------------------------------------------------------------------------------------------------------------//
    //               ____           ____ ____      ____ ____ ____      ____ ____ ____ ____ ____      ____           ____      ____ ____           ____             //
    //             /|    |        /|    |    |   /|    |    |    |   /|    |    |    |    |    |   /|    |        /|    |   /|    |    |        /|    |            //
    //          __|_|____|     __|_|____|____|  | |____|____|____|  | |____|____|____|____|____|  | |____|       | |____|  | |____|____|____   | |____|____        //
    //        /|    |___/    /|    |___/____/   |/|    |___/____/   |/____/____/|    |___/____/   |/|    |       |/|    |  |/|    |  |/|    |  |/____/|    |       //
    //     __|_|____|       | |____|____        | |____|____                  | |____|            | |____|       | |____|  | |____|__|_|____|       | |____|____   //
    //   /|    |___/        |/____/|    |       |/|    |    |                 |/|    |            |/|    |       |/|    |  |/|    |    |___/        |/____/|    |  //
    //  | |____|____             | |____|____   | |____|____|                 | |____|            | |____|       | |____|  | |____|____|               __|_|____|  //
    //  |/____/|    |            |/____/|    |  |/|    |___/                  |/|    |            |/|    |       |/|    |  |/|    |___/              /|    |___/   //
    //       | |____|____      ____ __|_|____|  | |____|____ ____             | |____|            | |____|____ __|_|____|  | |____|               __|_|____|       //
    //       |/____/|    |   /|    |    |___/   |/|    |    |    |            |/|    |            |/____/|    |    |___/   |/|    |             /|    |___/        //
    //            | |____|  | |____|____|       | |____|____|____|            | |____|                 | |____|____|       | |____|            | |____|            //
    //            |/____/   |/____/____/        |/____/____/____/             |/____/                  |/____/____/        |/____/             |/____/             //
    //                                                                                                                                                             //
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------//


    player_idle_right_sprite = new Sprite( "./assets/player_idle_right.png" , 64 , 64 , parent_app->get_renderer() , 24 , 24 );
    player_idle_left_sprite = new Sprite( "./assets/player_idle_left.png" , 64 , 64 , parent_app->get_renderer() , 24 , 24 );
    player_run_right_sprite = new Sprite( "./assets/player_run_right.png" , 64 , 64 , parent_app->get_renderer() , 24 , 24 );
    player_run_left_sprite = new Sprite( "./assets/player_run_left.png" , 64 , 64 , parent_app->get_renderer() , 24 , 24 );
    player_object_type = new ObjectType( player_create , player_step , nullptr , player_event , sizeof(player_variables_t) );

    unsigned int room_index = parent_app->create_room( 1 , 1 );
    parent_app->set_current_room( room_index );
    Room* room = parent_app->get_room( room_index );
    room->create_object( player_object_type );
    room->set_camera( 0 , room_camera_t{0,0,256,160} );
    TileLayer* _tile_layer = new TileLayer( new Sprite( "./assets/tiles.png" , 16 , 16 , parent_app->get_renderer() ) , 16 , 10 , 100 );
    room->set_tile_layer( 0 , _tile_layer );

    _tile_layer->set_tile(0,0,5);
    _tile_layer->set_tile(1,0,8);
    _tile_layer->set_tile(2,0,8);
    _tile_layer->set_tile(3,0,8);
    _tile_layer->set_tile(4,0,8);
    _tile_layer->set_tile(5,0,8);
    _tile_layer->set_tile(6,0,8);
    _tile_layer->set_tile(7,0,8);
    _tile_layer->set_tile(8,0,8);
    _tile_layer->set_tile(9,0,8);
    _tile_layer->set_tile(10,0,8);
    _tile_layer->set_tile(11,0,8);
    _tile_layer->set_tile(12,0,8);
    _tile_layer->set_tile(13,0,8);
    _tile_layer->set_tile(14,0,8);
    _tile_layer->set_tile(15,0,5);

    _tile_layer->set_tile(0,9,5);
    _tile_layer->set_tile(1,9,2);
    _tile_layer->set_tile(2,9,2);
    _tile_layer->set_tile(3,9,2);
    _tile_layer->set_tile(4,9,2);
    _tile_layer->set_tile(5,9,2);
    _tile_layer->set_tile(6,9,2);
    _tile_layer->set_tile(7,9,2);
    _tile_layer->set_tile(8,9,2);
    _tile_layer->set_tile(9,9,2);
    _tile_layer->set_tile(10,9,2);
    _tile_layer->set_tile(11,9,2);
    _tile_layer->set_tile(12,9,2);
    _tile_layer->set_tile(13,9,2);
    _tile_layer->set_tile(14,9,2);
    _tile_layer->set_tile(15,9,5);

    _tile_layer->set_tile(0,1,6);
    _tile_layer->set_tile(0,2,6);
    _tile_layer->set_tile(0,3,6);
    _tile_layer->set_tile(0,4,6);
    _tile_layer->set_tile(0,5,6);
    _tile_layer->set_tile(0,6,6);
    _tile_layer->set_tile(0,7,6);
    _tile_layer->set_tile(0,8,6);
    
    _tile_layer->set_tile(15,1,4);
    _tile_layer->set_tile(15,2,4);
    _tile_layer->set_tile(15,3,4);
    _tile_layer->set_tile(15,4,4);
    _tile_layer->set_tile(15,5,4);
    _tile_layer->set_tile(15,6,4);
    _tile_layer->set_tile(15,7,4);
    _tile_layer->set_tile(15,8,4);

    
    _tile_layer->set_tile(8,8,5);
    _tile_layer->set_tile(5,6,5);

    

    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
    //               ____                          ____           ____ ____      ____ ____ ____      ____ ____ ____ ____ ____      ____           ____      ____ ____           ____             //
    //             /|    |                       /|    |        /|    |    |   /|    |    |    |   /|    |    |    |    |    |   /|    |        /|    |   /|    |    |        /|    |            //
    //          __|_|____|                    __|_|____|     __|_|____|____|  | |____|____|____|  | |____|____|____|____|____|  | |____|       | |____|  | |____|____|____   | |____|____        //
    //        /|    |___/                   /|    |___/    /|    |___/____/   |/|    |___/____/   |/____/____/|    |___/____/   |/|    |       |/|    |  |/|    |  |/|    |  |/____/|    |       //
    //     __|_|____|                    __|_|____|       | |____|____        | |____|____                  | |____|            | |____|       | |____|  | |____|__|_|____|       | |____|____   //
    //   /|    |___/                   /|    |___/        |/____/|    |       |/|    |    |                 |/|    |            |/|    |       |/|    |  |/|    |    |___/        |/____/|    |  //
    //  | |____|____                __|_|____|                 | |____|____   | |____|____|                 | |____|            | |____|       | |____|  | |____|____|               __|_|____|  //
    //  |/____/|    |             /|    |___/                  |/____/|    |  |/|    |___/                  |/|    |            |/|    |       |/|    |  |/|    |___/              /|    |___/   //
    //       | |____|____      __|_|____|                    ____ __|_|____|  | |____|____ ____             | |____|            | |____|____ __|_|____|  | |____|               __|_|____|       //
    //       |/____/|    |   /|    |___/                   /|    |    |___/   |/|    |    |    |            |/|    |            |/____/|    |    |___/   |/|    |             /|    |___/        //
    //            | |____|  | |____|                      | |____|____|       | |____|____|____|            | |____|                 | |____|____|       | |____|            | |____|            //
    //            |/____/   |/____/                       |/____/____/        |/____/____/____/             |/____/                  |/____/____/        |/____/             |/____/             //
    //                                                                                                                                                                                           //
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


    return;
};

#endif
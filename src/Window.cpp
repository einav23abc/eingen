#ifndef Window_CPP
#define Window_CPP
#include "Window.hpp"


#include "App.cpp"


Window::Window( const char* title , int xpos , int ypos , int _width , int _height , int minWidth , int minHeight , bool resizable , bool fullscreen ){
    this->width = _width;
    this->height = _height;
    
    Uint32 flags = 0;
    if ( fullscreen ){
        flags = SDL_WINDOW_FULLSCREEN;
    };
    if ( resizable ){
        flags |= SDL_WINDOW_RESIZABLE;
    };

    if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ){
        this->isRunning = false;
        return;
    };
    
    std::cout << "Window Subsystems Initialised!..." << std::endl;
    this->window = SDL_CreateWindow( title , xpos , ypos , width , height , flags );
    SDL_SetWindowMinimumSize(this->window, minWidth , minHeight);
    
    if ( this->window ){
        std::cout << "Window Window Created!" << std::endl;
    };

    this->renderer = SDL_CreateRenderer( this->window , -1 , 0 );
    if ( this->renderer ){
        std::cout << "Window Renderer Created!" << std::endl;
    };

    textInput[0] = '\0';
    this->app = new App(this);

    this->isRunning = true;
    
    return;
};

Window::~Window(){
    return;
};

void Window::handleEvents(){

    SDL_Event event;
    while( SDL_PollEvent(&event) ){
        SDL_GetMouseState(&this->mouseX, &this->mouseY);

        LinkedListNode<eventFunct_t>* currentFunc = eventListenFuncList;
        LinkedListNode<void*>* currentListner = eventListenListnerList;
        while( currentFunc != nullptr && currentListner != nullptr ){
            eventFunct_t func = currentFunc->getValue();
            func( currentListner->getValue() , &event );
            currentFunc = currentFunc->getNext();
            currentListner = currentListner->getNext();
        };

        if ( event.type == SDL_QUIT ){
            isRunning = false;
        }else if ( event.type == SDL_WINDOWEVENT ){
            if ( event.window.event == SDL_WINDOWEVENT_RESIZED ){
                SDL_GetWindowSize( window, &width, &height );
            };
        }else if( event.type == SDL_MOUSEBUTTONDOWN ){
            
        }else if( event.type == SDL_MOUSEBUTTONUP ){
            
        }else if ( event.type == SDL_KEYDOWN ){
            //std::cout << SDL_GetKeyName( event.key.keysym.sym ) << std::endl;
            
            if ( strcmp( SDL_GetKeyName( event.key.keysym.sym ) , "Backspace" ) == 0 ){
                if ( strlen( textInput ) >= 1 ){
                    textInput[strlen( textInput )-1] = '\0';
                }else{
                    textInputBackspaces++;
                };
                std::cout << "\"" << textInput << "\" del:" << textInputBackspaces << std::endl;
            }else if ( strcmp( SDL_GetKeyName( event.key.keysym.sym ) , "Return" ) == 0 ){
                if ( event.key.keysym.mod & KMOD_LSHIFT ){
                    if ( strlen( textInput ) + 1 >= MAX_TEXT_INPUT_LENGTH ){
                        textInput[1] = '\0';
                        textInput[0] = '\3';
                        textInputBackspaces = 0;
                    }else{
                        textInput[strlen(textInput)+1] = '\0';
                        textInput[strlen(textInput)] = '\3';
                    };
                }else{
                    if ( strlen( textInput ) + 1 >= MAX_TEXT_INPUT_LENGTH ){
                        textInput[1] = '\0';
                        textInput[0] = '\n';
                        textInputBackspaces = 0;
                    }else{
                        textInput[strlen(textInput)+1] = '\0';
                        textInput[strlen(textInput)] = '\n';
                    };
                };
                std::cout << "\"" << textInput << "\" del:" << textInputBackspaces << std::endl;
            }else if ( strcmp( SDL_GetKeyName( event.key.keysym.sym ) , "Tab" ) == 0 ){
                if ( strlen( textInput ) + 4 >= MAX_TEXT_INPUT_LENGTH ){
                    strcpy( textInput , "    " );
                    textInputBackspaces = 0;
                }else{
                    strcat( textInput , "    " );
                };
                std::cout << "\"" << textInput << "\" del:" << textInputBackspaces << std::endl;
            };
        }else if ( event.type == SDL_KEYUP ){
            
        }else if ( event.type == SDL_TEXTINPUT ){
            if ( strlen( textInput ) + strlen(event.text.text) >= MAX_TEXT_INPUT_LENGTH ){
                strcpy( textInput , "" );
                textInputBackspaces = 0;
            };
            strcat(textInput, event.text.text );
            std::cout << "\"" << textInput << "\" del:" << textInputBackspaces << std::endl;
        }else {

        };
    };
    return;
};

void Window::update(){
    ((App*)this->app)->Update();
    return;
};

void Window::render(){
    SDL_SetRenderDrawColor( this->renderer , 0 , 0 , 0 , 255 );
    SDL_RenderClear(this->renderer);

    ((App*)this->app)->Render();

    SDL_RenderPresent(this->renderer);
    return;
};

void Window::clean(){
    delete ((App*)this->app);
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->renderer);
    SDL_Quit();
    std::cout << "Window Cleaned" << std::endl;
    return;
};


void Window::listenToEvent( void* listner , eventFunct_t funcptr ){
    if ( this->eventListenFuncList == nullptr ){
        this->eventListenFuncList = new LinkedListNode<eventFunct_t>(funcptr);
        this->eventListenFuncListTail = this->eventListenFuncList;
        this->eventListenListnerList = new LinkedListNode<void*>(listner);
        this->eventListenListnerListTail = this->eventListenListnerList;
        return;
    };
    this->eventListenFuncListTail->setNext( new LinkedListNode<eventFunct_t>(funcptr) );
    this->eventListenFuncListTail = this->eventListenFuncListTail->getNext();
    this->eventListenListnerListTail->setNext( new LinkedListNode<void*>(listner) );
    this->eventListenListnerListTail = this->eventListenListnerListTail->getNext();
    return;
};


#endif
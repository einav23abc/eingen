#ifndef Window_HPP
#define Window_HPP

class Window;

#include "LinkedList.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string.h>

typedef void (*eventFunct_t)(void* This,SDL_Event* event);

class Window {
    public:
        Window(const char* title,int xpos,int ypos,int _width,int _height,int minWidth,int minHeight,bool resizable,bool fullscreen);
        ~Window();
        void handleEvents();
        void update();
        void render();
        void clean();
        bool running(){ return isRunning; };
        SDL_Renderer* renderer;
#define MAX_TEXT_INPUT_LENGTH 12
        char textInput[MAX_TEXT_INPUT_LENGTH];
        int textInputBackspaces = 0;
        int width, height;
        int mouseX, mouseY;
        void listenToEvent( void* listner , eventFunct_t funcptr );
    private:
        LinkedListNode<eventFunct_t>* eventListenFuncList = nullptr;
        LinkedListNode<eventFunct_t>* eventListenFuncListTail = nullptr;
        LinkedListNode<void*>* eventListenListnerList = nullptr;
        LinkedListNode<void*>* eventListenListnerListTail = nullptr;
        
        bool isRunning;
        SDL_Window* window;
        void* app; /* supose to be (App*) app */
};

#include "App.hpp"
#include "Window.cpp"
#endif
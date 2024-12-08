#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_events.h>
#include <stdbool.h>

#include "vector3.h"
    
SDL_Window *window; 
SDL_Renderer *renderer;

int mouseXmotion, mouseYmotion;
int mouseX, mouseY;
bool clicking = false;
bool mouseMoving = false;
bool running = true;
bool keysPressed[SDL_NUM_SCANCODES];

point3 position;
vector3 dirFacing;

int userInput(){
    SDL_Event event;

    mouseMoving = false;
    while(SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                running = false;
                return 1;
                break;

            case SDL_KEYDOWN:
                keysPressed[event.key.keysym.scancode] = true;
            break;

            case SDL_KEYUP:
                keysPressed[event.key.keysym.scancode] = false;
            break;
            
            case SDL_MOUSEMOTION:
                mouseMoving = true;
                mouseXmotion = event.motion.xrel;       
                mouseYmotion = event.motion.yrel;       
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                //printf("X: %d, Y: %d\n", mouseX, mouseY);
            break;

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT){
                   clicking = true;
                }
            break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                   clicking = false;
                }
            break;
        }
    }
    return 0;
}

int updateState(){
    if(keysPressed[SDL_SCANCODE_Q]){
        running = false;
        return 1;
    }

    if(mouseMoving){
        //move camera
    }

    if(clicking){
        SDL_RenderDrawPoint(renderer, mouseX, mouseY);
    }
    return 0;
}

int initializeSDL(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "ERROR: SDL did not initialize properly: %s\n", SDL_GetError());
        return -1;
    }

     window = SDL_CreateWindow("Graphics", 
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1000, 1000, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    return 0;
}

void initializeState(){
    init(&position, 0, 0, 0);
    init(&dirFacing, 0, 0, -1);
}

int main(int argc, char *argv[]){
    if(initializeSDL() != 0){
        return -1;
    }

    initializeState();

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    while(running){
        if(userInput() != 0) break;
        if(updateState() != 0) break;
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

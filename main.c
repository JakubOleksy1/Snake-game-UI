#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "game.h"
//#include "sdl_screen.h"


#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
} SdlHandler;

bool load_media(SdlHandler *sdl) {
    sdl->background = IMG_LoadTexture(sdl->renderer, "images/background.png");

    if(!sdl->background) {
        fprintf(stderr, "Error creating texture.\n Error message: %s\n", IMG_GetError());
        return true;
    }
    return false;
}

void sdl_cleanUp(SdlHandler *sdl, int exit_status) {
    SDL_DestroyTexture(sdl->background);
    SDL_DestroyRenderer(sdl->renderer);
    SDL_DestroyWindow(sdl->window);
    IMG_Quit();
    SDL_Quit();
    exit(exit_status);
}

bool sdl_initialize(SdlHandler *sdl) {
    if(SDL_Init(SDL_INIT_EVERYTHING != 0)) {
        fprintf(stderr, "Error initializing SDL.\n Error message: %s\n", SDL_GetError());
        return true;
    }

    int img_init = IMG_Init(IMAGE_FLAGS);
    if((img_init & IMAGE_FLAGS) != IMAGE_FLAGS) {
        fprintf(stderr, "Error initializing SDL_image.\n Error message: %s\n", IMG_GetError());
        return true;
    }

    sdl->window = SDL_CreateWindow(WINDOW_TITLE, 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    SCREEN_WIDTH, 
                    SCREEN_HEIGHT, 
                    SDL_WINDOW_RESIZABLE);

    if(!sdl->window) {
        fprintf(stderr, "Error creating window.\n Error message: %s\n", SDL_GetError());
        return true;
    }

    sdl->renderer = SDL_CreateRenderer(sdl->window, -1, 0);
    
    if(!sdl->renderer) {
        fprintf(stderr, "Error creating renderer.\n Error message: %s\n", SDL_GetError());
        return true;
    }

    return false; 
}

int main(int argc, char *argv[]) {
    //srand(time(NULL));  
    
    //Game game;

    SdlHandler sdl = {
        .window = NULL,
        .renderer = NULL,
        .background = NULL
    };

    if(sdl_initialize(&sdl)) {
        sdl_cleanUp(&sdl, EXIT_FAILURE);
    }

    if(load_media(&sdl)) {
        sdl_cleanUp(&sdl, EXIT_FAILURE);
    }
    // game_init(&game);
    
    while(true) {

        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {    
                case SDL_QUIT:
                    sdl_cleanUp(&sdl, EXIT_SUCCESS);
                    break;
                
                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            sdl_cleanUp(&sdl, EXIT_SUCCESS);
                            break;

                        default:
                            break;
                    }

                default:
                    break;
            }
        }
        SDL_RenderClear(sdl.renderer);

        SDL_RenderCopy(sdl.renderer, sdl.background, NULL, NULL);

        SDL_RenderPresent(sdl.renderer);

        SDL_Delay(16);
    }

    //while(game.state == PLAYING) {
    //     game_clearScreen(); 
    //     game_input(&game);
    //     game_update(&game);
    //     game_render(&game);
    //     Sleep(100);
    //}
    // game_gameOver_msg(&game);
    // game_end(&game); 
    sdl_cleanUp(&sdl, EXIT_SUCCESS);
    return 0;
}

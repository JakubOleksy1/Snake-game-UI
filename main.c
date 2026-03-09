#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "game.h"
#include "sdl_screen.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define TEXT_SIZE 80
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    TTF_Font *text_font;
    SDL_Color text_color;
    SDL_Rect text_rect;
    SDL_Texture *text_image;
    int text_xval;
    int text_yval;
    SDL_Texture *sprite_image;
    SDL_Rect sprite_rect;
    int sprite_vel;
    const uint8_t *keystate;
} SdlHandler;

void text_update(SdlHandler *sdl) {
    sdl->text_rect.x += sdl->text_xval;
    sdl->text_rect.y += sdl->text_yval; 
    if(sdl->text_rect.x + sdl->text_rect.w > SCREEN_WIDTH) {
        sdl->text_xval *= -1;
    }
    if(sdl->text_rect.x < 0) {
        sdl->text_xval *= -1;
    }
    if(sdl->text_rect.y + sdl->text_rect.h > SCREEN_HEIGHT) {
        sdl->text_yval *= -1;
    }
    if(sdl->text_rect.y < 0) {
        sdl->text_yval *= -1;
    }
}

void sprite_update(SdlHandler *sdl) {
    if(sdl->keystate[SDL_SCANCODE_LEFT] || sdl->keystate[SDL_SCANCODE_A]) {
        sdl->sprite_rect.x -= sdl->sprite_vel;
    }
    if(sdl->keystate[SDL_SCANCODE_RIGHT] || sdl->keystate[SDL_SCANCODE_D]) {
        sdl->sprite_rect.x += sdl->sprite_vel;
    }
    if(sdl->keystate[SDL_SCANCODE_UP] || sdl->keystate[SDL_SCANCODE_W]) {
        sdl->sprite_rect.y -= sdl->sprite_vel;
    }
    if(sdl->keystate[SDL_SCANCODE_DOWN] || sdl->keystate[SDL_SCANCODE_S]) {
        sdl->sprite_rect.y += sdl->sprite_vel;
    }
}

bool load_media(SdlHandler *sdl) {
    sdl->background = IMG_LoadTexture(sdl->renderer, "images/background.png");

    if(!sdl->background) {
        fprintf(stderr, "Error creating texture.\n Error message: %s\n", IMG_GetError());
        return true;
    }

    sdl->text_font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_SIZE); 
    if(!sdl->text_font) {
        fprintf(stderr, "Error creating font.\n Error message: %s\n", TTF_GetError());
        return true;
    }

    SDL_Surface *surface = TTF_RenderText_Blended(sdl->text_font, "HELLO", sdl->text_color);
    if(!surface) {
        fprintf(stderr, "Error creating surface.\n Error message: %s\n", SDL_GetError());
        return true;
    }

    sdl->text_rect.w = surface->w;
    sdl->text_rect.h = surface->h;
    
    sdl->text_image = SDL_CreateTextureFromSurface(sdl->renderer, surface);
    SDL_FreeSurface(surface);
    if(!sdl->text_image) {
        fprintf(stderr, "Error creating texture.\n Error message: %s\n", SDL_GetError());
        return true;
    }
    
    sdl->sprite_image = IMG_LoadTexture(sdl->renderer, "images/Snake_head_u.png");
    if(!sdl->sprite_image) {
        fprintf(stderr, "Error loading texture.\n Error message: %s\n", IMG_GetError());
        return true;
    }

    if(SDL_QueryTexture(sdl->sprite_image, NULL, NULL, &sdl->sprite_rect.w, &sdl->sprite_rect.h)) {
        fprintf(stderr, "Error querying texture.\n Error message: %s\n", SDL_GetError());
        return true;
    }
    return false;
}

void sdl_cleanUp(SdlHandler *sdl, int exit_status) {
    SDL_DestroyTexture(sdl->sprite_image);
    SDL_DestroyTexture(sdl->text_image);
    TTF_CloseFont(sdl->text_font);
    SDL_DestroyTexture(sdl->background);
    SDL_DestroyRenderer(sdl->renderer);
    SDL_DestroyWindow(sdl->window);
    TTF_Quit();
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

    if(TTF_Init()) {
        fprintf(stderr, "Error initializing SDL_ttf.\n Error message: %s\n", TTF_GetError());
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
    srand(time(NULL));  
    
    //Game game;

    SdlHandler sdl = {
        .window = NULL,
        .renderer = NULL,
        .background = NULL,
        .text_font = NULL,
        .text_color = {255, 255, 255, 255},
        .text_rect = {(SCREEN_WIDTH/2), (SCREEN_HEIGHT/4), 0, 0},
        .text_image = NULL,
        .text_xval = 3,
        .text_yval = 3,
        .sprite_image = NULL,
        .sprite_rect = {0, 0, 0, 0},
        .sprite_vel = 5,
        .keystate = SDL_GetKeyboardState(NULL),
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
                /*case SDL_SCANCODE_SPACE:
                    SDL_SetRenderDrawColor(sdl.renderer, rand() % 256,
                                            rand() % 256, rand() % 256, 255);*/
                        default:
                            break;
                    }

                default:
                    break;
            }
        }

        text_update(&sdl);

        sprite_update(&sdl);

        SDL_RenderClear(sdl.renderer);

        SDL_RenderCopy(sdl.renderer, sdl.background, NULL, NULL);

        SDL_RenderCopy(sdl.renderer, sdl.text_image, NULL, &sdl.text_rect);

        SDL_RenderCopy(sdl.renderer, sdl.sprite_image, NULL, &sdl.sprite_rect);

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

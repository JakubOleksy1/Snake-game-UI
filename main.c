#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "game.h"

bool std_initialize(SdlHandler *sdl) {
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initializing SDL.\n Error message: %s\n", SDL_GetError());
        return true;
    }
}

int main() {
    srand(time(NULL));  
    
    Game game;

    SdlHandler sdl = {
        .window = NULL,
        .renderer = NULL
    };

    sdl_initialize(&sdl);

    game_init(&game);
    
    while(game.state == PLAYING) {
        game_clearScreen(); 
        game_input(&game);
        game_update(&game);
        game_render(&game);
        Sleep(100);
    }
    game_gameOver_msg(&game);
    game_end(&game); 
    return 0;
}

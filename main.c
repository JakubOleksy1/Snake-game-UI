#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "game.h"

int main() {
    srand(time(NULL));  
    
    Game game;
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

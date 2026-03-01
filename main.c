#include <stdio.h>
#include <stdlib.h>

#include "game.h"

void clearScreen() {
    COORD cursorPosition = {0, 0};
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hStdOut, cursorPosition);
}

int main() {
    srand(time(NULL));  
    
    Game game;
    game_init(&game);
    
    while(game.state == PLAYING) {
        clearScreen(); 
        game_input(&game);
        game_update(&game);
        game_render(&game);
        Sleep(100);
    }
    game_gameOver_msg(&game);
    game_end(&game); 
    return 0;
}

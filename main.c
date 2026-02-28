#include "snake.h"
#include "food.h"
#include "map.h"
#include "input_console.h"
#include "render_console.h"
#include "game.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void clearScreen() {
    COORD cursorPosition = {0, 0};
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hStdOut, cursorPosition);
}

void draw(const char (*map)[MAP_SIZE_X], const Snake *snake, const Food *food) {  
    char screen[MAP_SIZE_Y][MAP_SIZE_X];

    for (int i = 0; i < MAP_SIZE_Y; i++) {
        for (int j = 0; j < MAP_SIZE_X; j++) {
            screen[i][j] = map[i][j];
        }
    }

    for (int k = 0; k < snake->length; k++) {
            screen[snake->body[k].coord_y][snake->body[k].coord_x] = TILE_SNAKE;
    }

    screen[food->coord_y][food->coord_x] = TILE_FOOD;

    for (int i = 0; i < MAP_SIZE_Y; i++) {
        for (int j = 0; j < MAP_SIZE_X; j++) {
            printf("%c", screen[i][j]);
        }
        printf("\n");
    }
}


void checkInput(Snake *snake) {
    if (_kbhit()) {
        int c = _getch();
        if (c == 0 || c == 0xE0) {
            c = _getch();
            if (c == 72 && snake->nextDirection != DOWN) {
                snake->nextDirection = UP; 
                return;
            }
            if (c == 80 && snake->nextDirection != UP) { 
                snake->nextDirection = DOWN; 
                return;
            }
            if (c == 75 && snake->nextDirection != RIGHT) { 
                snake->nextDirection = LEFT; 
                return;
            }
            if (c == 77 && snake->nextDirection != LEFT) { 
                snake->nextDirection = RIGHT;
                return;
            }
        } else {                            
            if ((c == 'w' || c == 'W') && snake->nextDirection != DOWN) {
                snake->nextDirection = UP;
                return;
            }
            if ((c == 's' || c == 'S') && snake->nextDirection != UP) { 
                snake->nextDirection = DOWN;
                return;
            }
            if ((c == 'a' || c == 'A') && snake->nextDirection != RIGHT) { 
                snake->nextDirection = LEFT;
                return;
            }
            if ((c == 'd' || c == 'D') && snake->nextDirection != LEFT) { 
                snake->nextDirection = RIGHT;
                return;
            }
        }
    }
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
#include <windows.h>
#include <stdio.h>

#include "render_console.h"
#include "game.h"

void draw_clearScreen() {
    COORD cursorPosition = {0, 0};
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hStdOut, cursorPosition);
}

void draw(Game *game) {  
    Snake *snake = &game->snake;
    Food *food = &game->food;
    GameState *state = &game->state;
    int *points = &game->points;

    char screen[MAP_SIZE_Y][MAP_SIZE_X];

    for (int i = 0; i < MAP_SIZE_Y; i++) {
        for (int j = 0; j < MAP_SIZE_X; j++) {
            screen[i][j] = game->map[i][j];
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

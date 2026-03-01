#include "render_console.h"

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

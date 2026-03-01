#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "snake.h"
#include "food.h"
#include "map.h"
#include "config.h"
#include "input_console.h"
#include "render_console.h"

typedef enum {
    MENU,
    PLAYING,
    PAUSED,
    GAMEOVER
} GameState;

typedef struct {
    Snake snake;
    Food food;
    int points;
    char map[MAP_SIZE_Y][MAP_SIZE_X];
    GameState state;
} Game;

void game_init(Game *game);
void game_input(Game *game);
void game_update(Game *game);
void game_render(Game *game);
void game_gameOver_msg(Game *game);
void game_end(Game *game);
void checkRules(Game *game);        
void spawnFood(Game *game);

#endif /* GAME_H */

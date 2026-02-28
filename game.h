#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "food.h"
#include "map.h"
#include "config.h"

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

#endif /* GAME_H */
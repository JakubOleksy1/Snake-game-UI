#include <stdbool.h>
#include <stdio.h>

#include "game.h"
#include "input_console.h"
#include "render_console.h"

void game_init(Game *game) {
    game->state = PLAYING;
    game->points = 0;
    
    initializeSnake(&game->snake);
    
    createMap(game->map);

    initializeFood(&game->food);
    
    if (!spawnFood(&game->food, &game->snake)) {
        game->state = GAMEOVER;
    }
}

void game_input(Game *game) {
    checkInput(&game->snake);
}

void game_update(Game *game) {
    moveSnake(&game->snake);
    checkRules(game);        
    if (!spawnFood(&game->food, &game->snake)) {
        game->state = GAMEOVER;
    }
}

void game_render(Game *game) {
    draw(game);
}

void game_clearScreen() {
    draw_clearScreen();
}

void game_gameOver_msg(Game *game) {
    printf("Game over! \n Your points: %d", game->points);
}

void game_end(Game *game) {
    destroySnake(&game->snake);
}

void checkRules(Game *game) {
    
    //added aliases because i did not want to edit whole function with its names.
    Snake *snake = &game->snake;
    Food *food = &game->food;
    GameState *state = &game->state;
    int *points = &game->points;

    //rule 1 wall collision
    if( (snake->body[0].coord_x == 0 || snake->body[0].coord_x == MAP_SIZE_X - 1) ||
        (snake->body[0].coord_y == 0 || snake->body[0].coord_y == MAP_SIZE_Y - 1) )
    {
        *state = GAMEOVER;
        return;
    }

    //rule 2 snake hits itself
    for(int i = 1; i < snake->length; i++) {
        if(snake->body[0].coord_x == snake->body[i].coord_x && snake->body[0].coord_y == snake->body[i].coord_y) {
            *state = GAMEOVER;
            return;
        }
    }

    //food is eaten 
    if((snake->body[0].coord_x == food->coord_x) && (snake->body[0].coord_y == food->coord_y)) {
        food->is_eaten = true;
        (*points)++;
        snake->growPending = true;
        
    }
    //max points achieved - player won
    if((*points) == ((MAP_SIZE_Y - 2) * (MAP_SIZE_X - 2)) - STARTING_LENGTH) {
        printf("You won!");
        *state = GAMEOVER;
        return;
    }
}


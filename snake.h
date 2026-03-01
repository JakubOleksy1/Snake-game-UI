#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>

#include "config.h"

typedef enum {
    RIGHT,
    DOWN,
    LEFT,
    UP
} Direction;

typedef struct {
    int coord_x;
    int coord_y;
} Snake_coord;

typedef struct Snake {
    int length;
    int capacity;
    Direction direction;
    Direction nextDirection;
    bool growPending;
    Snake_coord *body;
} Snake;

void initializeSnake(Snake *snake);
void moveSnake(Snake *snake);
void destroySnake(Snake *snake);

#endif /* SNAKE_H */

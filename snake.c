#include "snake.h"

void initializeSnake(Snake *snake) { 
    snake->length = STARTING_LENGTH;
    snake->capacity = (MAP_SIZE_Y - 2) * (MAP_SIZE_X - 2);
    snake->direction = UP;
    snake->nextDirection = snake->direction;
    snake->body = (Snake_coord *)malloc(snake->capacity * sizeof(Snake_coord));

    if (snake->body == NULL) {
        printf("Allocation Failed");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < snake->length; i++) {
        snake->body[i].coord_x = (MAP_SIZE_X/2) - i;
        snake->body[i].coord_y = (MAP_SIZE_Y/2);
    }
}

void moveSnake(Snake *snake) {
    snake->direction = snake->nextDirection;

    for(int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    
    if(snake->direction == RIGHT) { snake->body[0].coord_x++; }
    if(snake->direction == DOWN) { snake->body[0].coord_y++; }
    if(snake->direction == LEFT) { snake->body[0].coord_x--; }
    if(snake->direction == UP) { snake->body[0].coord_y--; }

    if(snake->growPending == true && snake->length < snake->capacity) {
        snake->length++;
        snake->growPending = false;
        snake->body[snake->length - 1] = snake->body[snake->length - 2];
    }
}

void destroySnake(Snake *snake) {
    free(snake->body);
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
// #include "snake.c"
// #include "snake.h"

// #include "food.c"
// #include "food.h"

// #include "map.c"
// #include "map.h"

#define TILE_WALL '#'
#define TILE_EMPTY ' '
#define TILE_SNAKE 'O'
#define TILE_FOOD '*'

#define MAP_SIZE_X 21
#define MAP_SIZE_Y 21

typedef struct {
    int coord_x;
    int coord_y;
    bool is_eaten;
} Food;

#define STARTING_LENGTH 3

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

typedef struct {
    int length;
    int capacity;
    Direction direction;
    Direction nextDirection;
    bool growPending;
    Snake_coord *body;
} Snake;

typedef enum {
    MENU,
    PLAYING,
    PAUSED,
    GAMEOVER
} GameState;

void clearScreen() {
    COORD cursorPosition = {0, 0};
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hStdOut, cursorPosition);
}

static inline void spawnFood(Food *food, const Snake *snake, GameState *state) {
    if (food->is_eaten == true && snake->length < ((MAP_SIZE_Y - 2) * (MAP_SIZE_X - 2))) 
    {   
        int attempts = 0;
        int maxAttempts = MAP_SIZE_X * MAP_SIZE_Y;
        bool confirmSpawn = false;
        while(!confirmSpawn && attempts < maxAttempts) {
            food->coord_x = (rand() % (MAP_SIZE_X-2)) + 1;
            food->coord_y = (rand() % (MAP_SIZE_Y-2)) + 1;
            bool collision = false;
                for(int i = 0; i < snake->length; i++) {
                    if(food->coord_x == snake->body[i].coord_x && food->coord_y == snake->body[i].coord_y) {
                        collision = true;
                        break;
                    }
                }
                if(collision == false) {
                    confirmSpawn = true;
                    food->is_eaten = false;
                }
            attempts++;
        }
        if(!confirmSpawn) {
            *state = GAMEOVER;
        }
    }
}

static inline void moveSnake(Snake *snake) {
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

static inline void createMap(char (*map)[MAP_SIZE_X]) {
    for (int i = 0; i < MAP_SIZE_Y; i++) {
        for (int j = 0; j < MAP_SIZE_X; j++) {
            if ((i == 0) || 
                (j == 0) || 
                (i == (MAP_SIZE_Y - 1)) || 
                (j == (MAP_SIZE_X - 1)) )
            {
                map[i][j] = TILE_WALL;
            } 
            else 
            {
                map[i][j] = TILE_EMPTY;
            }
        }
    }
}

static inline void draw(const char (*map)[MAP_SIZE_X], const Snake *snake, const Food *food) {  
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

static inline void checkRules(Snake *snake, Food *food, GameState *state, int *points) {
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

static inline void checkInput(Snake *snake) {
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

static inline void initializeSnake(Snake *snake) { 
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

static inline void initializeFood(Food *food) {
    food->is_eaten = true;
}

static inline void destroySnake(Snake *snake) {
    free(snake->body);
}

int main() {
    srand(time(NULL));  
    
    GameState state = PLAYING;
    int points = 0;

    Snake snake;
    initializeSnake(&snake);
    
    char map[MAP_SIZE_Y][MAP_SIZE_X];
    createMap(map);

    Food food;
    initializeFood(&food);
    spawnFood(&food, &snake, &state);
    
    while(state == PLAYING) {
        clearScreen();
        checkInput(&snake);   
        moveSnake(&snake);
        checkRules(&snake, &food, &state, &points);        
        spawnFood(&food, &snake, &state);
        draw(map, &snake, &food);    
        Sleep(100);
    }

    printf("Game over! \n Your points: %d", points);
    destroySnake(&snake); 
    return 0;
}
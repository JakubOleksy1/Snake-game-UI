#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>

#define MAP_SIZE_X 21
#define MAP_SIZE_Y 21

#define CLEAR_SCREEN() printf("\033[2J\033[H")

typedef struct {
    int coord_x;
    int coord_y;
} Snake_coord;

typedef struct {
    int length;
    int capacity;
    int direction;
    Snake_coord *body;
} Snake;

typedef struct {
    int coord_x;
    int coord_y;
    bool is_eaten;
} Food;

typedef struct {
    int mapField[MAP_SIZE_Y][MAP_SIZE_X];
} Map;

void clearScreen() {
    COORD cursorPosition = {0, 0};
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hStdOut, cursorPosition);
}

static inline void spawnFood(Food *food, Snake *snake) {
    if (food->is_eaten == true) 
    {
        bool confirmSpawn = false;
        while(confirmSpawn == false) {
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
            }
        }
        food->is_eaten = false;
    }
}

static inline void moveSnake(Snake *snake) {
    for(int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    
    if(snake->direction == 0) { snake->body[0].coord_x++; }
    if(snake->direction == 1) { snake->body[0].coord_y++; }
    if(snake->direction == 2) { snake->body[0].coord_x--; }
    if(snake->direction == 3) { snake->body[0].coord_y--; }
}

static inline void createMap(Map *map) {
    for (int i = 0; i < MAP_SIZE_Y; i++) {
        for (int j = 0; j < MAP_SIZE_X; j++) {
            if ((i == 0) || 
                (j == 0) || 
                (i == (MAP_SIZE_Y - 1)) || 
                (j == (MAP_SIZE_X - 1)) )
            {
                map->mapField[i][j] = 1;
            } 
            else 
            {
                map->mapField[i][j] = 0;
            }
        }
    }
}

static inline void draw(const Map *map, const Snake *snake, const Food *food) {  
    for (int i = 0; i < MAP_SIZE_Y; i++) {
        for (int j = 0; j < MAP_SIZE_X; j++) {
            bool printed = false;
            
            for (int k = 0; k < snake->length; k++) {
                if(i == snake->body[k].coord_y && j == snake->body[k].coord_x) {
                    printf("%d", 3);
                    printed = true;
                    break;
                }
            }
            
            if(!printed && i == food->coord_y && j == food->coord_x) {
                printf("%d",4);
                printed = true;
            }
            
            if(!printed)
            {
                printf("%d", map->mapField[i][j]);
            }
        }
        printf("\n");
    }
}

static inline void checkRules(const Map *map, Snake *snake, Food *food, bool *gameOver, int *points) {
    //rule 1 wall collision
    if( (snake->body[0].coord_x == 0 || snake->body[0].coord_x == MAP_SIZE_X - 1) ||
        (snake->body[0].coord_y == 0 || snake->body[0].coord_y == MAP_SIZE_Y - 1) )
    {
        *gameOver = true;
    }
    //rule 2 snake hits itself
    for(int i = 1; i < snake->length; i++) {
        if(snake->body[0].coord_x == snake->body[i].coord_x && snake->body[0].coord_y == snake->body[i].coord_y) {
            *gameOver = true;
        }
    }

    //food is eaten 
    if((snake->body->coord_x == food->coord_x) && (snake->body->coord_y == food->coord_y)) {
        food->is_eaten = true;
        (*points)++;
        snake->length++;
        
    }
    //max points achieved - player won
    if((*points) == ((MAP_SIZE_Y - 2) * (MAP_SIZE_X - 2))) {
        printf("You won!");
        *gameOver = true;
    }
}

int main() {
    srand(time(NULL));  
    Map map;
    
    Food food;
    food.is_eaten = true;
    
    Snake snake;   
    snake.length = 3;
    snake.capacity = (MAP_SIZE_Y - 2) * (MAP_SIZE_X - 2);
    snake.direction = 3;
    snake.body = (Snake_coord *)malloc(snake.capacity * sizeof(Snake_coord));
    for(int i = 0; i < snake.length; i++) {
        snake.body[i].coord_x = (MAP_SIZE_X/2) - i;
        snake.body[i].coord_y = (MAP_SIZE_Y/2) - i ;
    }
    
    createMap(&map);
    spawnFood(&food, &snake);
    
    bool gameOver = false;
    int points = 0;
    /***********
    * 0 - right
    * 1 - down
    * 2 - left
    * 3 - up
    ************/
    while(gameOver == false) {
        clearScreen();
        draw(&map, &snake, &food);
        
        if (_kbhit()) {
            int c = _getch();
            if (c == 0 || c == 0xE0) {         // arrow key prefix
                c = _getch();
                if (c == 72 && snake.direction != 1) snake.direction = 3; // up arrow -> direction 3 (up)
                if (c == 80 && snake.direction != 3) snake.direction = 1; // down arrow -> direction 1 (down)
                if (c == 75 && snake.direction != 0) snake.direction = 2; // left arrow -> direction 2 (left)
                if (c == 77 && snake.direction != 2) snake.direction = 0; // right arrow -> direction 0 (right)
            } else {                            
                if ((c == 'w' || c == 'W') && snake.direction != 1) snake.direction = 3;
                if ((c == 's' || c == 'S') && snake.direction != 3) snake.direction = 1;
                if ((c == 'a' || c == 'A') && snake.direction != 0) snake.direction = 2;
                if ((c == 'd' || c == 'D') && snake.direction != 2) snake.direction = 0;
            }
        }
        
        moveSnake(&snake);
        checkRules(&map, &snake, &food, &gameOver, &points);
        spawnFood(&food, &snake);
        Sleep(100);
    }
    printf("Game over! \n Your points: %d", points);
    free(snake.body);
    return 0;
}

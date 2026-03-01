#include <stdlib.h>

#include "food.h"
#include "snake.h"

void initializeFood(Food *food) {
    food->is_eaten = true;
}

bool spawnFood(Food *food, const struct Snake *snake) {
    //added aliases because i did not want to edit whole function with its names.

    if (food->is_eaten == true && snake->length < snake->capacity) 
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
            return false;
        }
    }
    return true;
}

#ifndef FOOD_H
#define FOOD_H

#include <stdbool.h>

typedef struct {
    int coord_x;
    int coord_y;
    bool is_eaten;
} Food;

struct Snake;

void initializeFood(Food *food);
bool spawnFood(Food *food, const struct Snake *snake);

#endif /* FOOD_H */

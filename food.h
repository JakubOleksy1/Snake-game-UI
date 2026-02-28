#ifndef FOOD_H
#define FOOD_H

#include <stdbool.h>
typedef struct {
    int coord_x;
    int coord_y;
    bool is_eaten;
} Food;

void initializeFood(Food *food);

#endif /* FOOD_H */
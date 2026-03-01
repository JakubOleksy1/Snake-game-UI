#ifndef RENDER_CONSOLE_H
#define RENDER_CONSOLE_H

#include "snake.h"
#include "food.h"
#include "map.h"
#include "config.h"

void draw(const char (*map)[MAP_SIZE_X], const Snake *snake, const Food *food); 

#endif /* RENDER_CONSOLE_H */

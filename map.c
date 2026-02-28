#include "map.h"

void createMap(char (*map)[MAP_SIZE_X]) {
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
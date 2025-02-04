#include "map.h"
#include <raylib.h>


void drawMap(uint8_t map [MAP_SIZE][MAP_SIZE]){
    for (size_t x = 0; x < MAP_SIZE; x++)
    {
        for (size_t y = 0; y < MAP_SIZE; y++)
        {
            if(map[x][y] == 1){
                DrawRectangle(x * WALL_SIZE, y * WALL_SIZE, WALL_SIZE, WALL_SIZE, WHITE);
            }
        }
    }
}
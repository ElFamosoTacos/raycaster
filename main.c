#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <raylib.h>
#include "map.h"
#include "player.h"
#include "render.h"

#define WINDOW_NAME "Simple Raycasting"

int main(){
    int toggleMap = 1; // switch between 2d and "3d"

    uint8_t map [MAP_SIZE][MAP_SIZE] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    Player player;
    initPlayer(&player);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME);

    while(!WindowShouldClose())
    {
        BeginDrawing();

            playerHandleInput(&player, map);
            // calculate rays from the player pov
            ArrayOfRay* flashLight = castFlashLight(&player, map);

            if(toggleMap == 1){
                drawMap(map);
                drawPlayer(&player);
                playerDrawDirection(&player);
                drawFlashLight(&player, flashLight);
            }
            else{
                drawScreen(flashLight, SCREEN_WIDTH, SCREEN_HEIGHT, &player);
            }
            
            ClearBackground(BLACK);
            free(flashLight->rayList);
            free(flashLight);
        EndDrawing();

        if(IsKeyDown(KEY_M)){
            if(toggleMap == 1){
                toggleMap = 0;
            }
            else{
                toggleMap = 1;
            }
        }
    }

    return 0;
}
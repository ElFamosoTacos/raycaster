#ifndef PLAYER_H
#define PLAYER_H
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include "map.h"
#include <math.h>

#define ROTATION_SPEED 0.001
#define PLAYER_SPEED 100
#define PLAYER_SIZE 10
#define NUMBER_OF_RAYS 100

typedef struct Player
{
    float posX, posY;
    float angle;
    float fov;
    Vector2 direction;
} Player;

typedef struct RLRay{
    Vector2 point;
    float distance;
} RLRay;

typedef struct ArrayOfRay{
    RLRay* rayList;
    int numberOfRays;
} ArrayOfRay;


// PLAYER RELATED FUNCTION
int initPlayer(Player* player);
void drawPlayer(Player* player);
void updateDirection (Player* player);
void playerTurnLeft  (Player* player);
void playerTurnRight (Player* player);
void playerGoForward (Player* player);
void playerGoBackward (Player* player);
void playerDrawDirection(Player* player);
int isPlayerCollidingWithMap(Player* player, uint8_t map[MAP_SIZE][MAP_SIZE]);
void playerHandleInput(Player* player, uint8_t map[MAP_SIZE][MAP_SIZE]);

// RAY CALCULATION FROM THE PLAYER
RLRay castRay(Player* player, uint8_t map[MAP_SIZE][MAP_SIZE], float offsetAngle);
ArrayOfRay* castFlashLight(Player* player, uint8_t map[MAP_SIZE][MAP_SIZE]);


// DRAWING FUNCTION FOR RAYS
void drawRay(Player* player, Vector2* rayPoint);
void drawFlashLight(Player* player, ArrayOfRay* tabPoint);

#endif
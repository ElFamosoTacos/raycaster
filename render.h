#ifndef RENDER_H
#define RENDER_H
#include "player.h"
#include <raylib.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

// draw a simple column of pixels
void drawColumn(int posX, int height, int size);
// calculate the size of a wall from a distance and fov
int calculateSize(float distance, int screenWidth, float fov);

// draw the "3d" view
void drawScreen(ArrayOfRay* rays, int screenWidth, int screenHeight, Player* player);

#endif
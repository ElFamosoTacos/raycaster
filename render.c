#include "render.h"


void drawColumn(int posX, int height, int size){
    int halfScreen  = height / 2;
    int segmentSize = size / 2;
    int topSize = halfScreen - segmentSize;
    int botSize = halfScreen + segmentSize;

    // we draw the line from the center of the screen
    // so one from center of screen to the top
    // another one from the center to the bottom
    DrawLine(posX, halfScreen, posX, topSize, ORANGE);
    DrawLine(posX, halfScreen, posX, botSize, ORANGE);
}

int calculateSize(float distance, int screenWidth, float fov){
    int projectionDistance = (screenWidth/2) / (tanf(fov/2));
    int wallHeight = (20*projectionDistance) / distance;

    return wallHeight;
}

void drawScreen(ArrayOfRay* rays, int screenWidth, int screenHeight, Player* player){
    int indiceX = 0;
    int numberOfRays = rays->numberOfRays;
    int columnWidth = screenWidth / rays->numberOfRays;

    for(int i = 0; i < numberOfRays; i++)
    {
        do{
            int size = calculateSize(rays->rayList[i].distance, screenWidth, player->fov);
            drawColumn(indiceX, screenHeight, size);

            indiceX++;
        }while (indiceX % columnWidth != 0);   
    }
}
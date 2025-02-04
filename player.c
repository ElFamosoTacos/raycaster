#include "player.h"

int initPlayer(Player* player){
    if(player != NULL){
    player->angle       = 0;      // in radiant
    player->fov         = PI/2;   // 90°
    player->direction.x = cos(0);
    player->direction.y = sin(0);
    player->posX        = 90;     // default start position
    player->posY        = 90;
    }
    else{
        return 1;
    }

    return 0;
}

void drawPlayer(Player* player){
    if(player != NULL){
        DrawCircle(player->posX, player->posY, PLAYER_SIZE, YELLOW);
    }
}


// called when the angle of the player is updated, then update the vector of direction
void updateDirection(Player* player){
    if(player != NULL){
        player->direction.x = cos(player->angle);
        player->direction.y = sin(player->angle);
    }
}

void playerTurnLeft (Player* player){
    if(player != NULL){
        player->angle -= ROTATION_SPEED;

        // make sure that we loop on a trigonometric circle
        if (player->angle < 0) {
            player->angle += 2 * PI;
        }

        updateDirection(player);
    }
}

void playerTurnRight (Player* player){
    if(player != NULL){
        player->angle += ROTATION_SPEED;

        // make sure that we loop on a trigonometric circle
        if (player->angle >= 2 * PI) {
            player->angle -= 2 * PI;
        }

        updateDirection(player);
    }
}

void playerGoForward (Player* player){
    if(player != NULL){
        float delta = GetFrameTime();
        player->posX += player->direction.x * (PLAYER_SPEED * delta);
        player->posY += player->direction.y * (PLAYER_SPEED * delta);
    }
}

void playerGoBackward (Player* player){
    if(player != NULL){
        float delta = GetFrameTime();
        player->posX -= player->direction.x * (PLAYER_SPEED * delta);
        player->posY -= player->direction.y * (PLAYER_SPEED * delta);
    }
}


// fucntion to show the direction of the player in 2d mode
void playerDrawDirection(Player* player){
    if(player != NULL){

        int distanceDirection = 30;
        int xLine = player->posX + (player->direction.x * distanceDirection);
        int yLine = player->posY + (player->direction.y * distanceDirection);

        DrawLine(player->posX, player->posY, xLine, yLine, RED);
    }
}


// check collision of the player with the map
int isPlayerCollidingWithMap(Player* player, uint8_t map[MAP_SIZE][MAP_SIZE]){
    int hasHit = 0;

    // go through the map
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            // if there is a wall
            if(map[i][j] == 1){
                // we calculate the hitbox of the wall at this position
                Rectangle wallHitBox = {
                    .x      = i * WALL_SIZE,
                    .y      = j * WALL_SIZE,
                    .width  = WALL_SIZE,
                    .height = WALL_SIZE,
                };

                // if hitbox of wall and player collides
                if(CheckCollisionCircleRec((Vector2){.x = player->posX, .y = player->posY}, PLAYER_SIZE, wallHitBox)){
                    hasHit = 1;
                }
            }
        }
    }
    
    return hasHit;
}

// handles movements input
void playerHandleInput(Player* player, uint8_t map[MAP_SIZE][MAP_SIZE]){
    if(player != NULL){
        if(IsKeyDown(KEY_A)){
            playerTurnLeft(player);
        }
        if(IsKeyDown(KEY_D)){
            playerTurnRight(player);
        }
        if(IsKeyDown(KEY_W)){
            playerGoForward(player);
            // if player collides with map, set back his position
            if(isPlayerCollidingWithMap(player, map)){
                playerGoBackward(player);
            }
        }
        if(IsKeyDown(KEY_S)){
            playerGoBackward(player);
            // if player collides with map, set back his position
            if(isPlayerCollidingWithMap(player, map)){
                playerGoForward(player);
            }
        }
    }
}



// calculate a ray from the player -> to a wall
// no DDA algorithm
RLRay castRay(Player* player, uint8_t map[MAP_SIZE][MAP_SIZE], float offsetAngle){
    Vector2 rayPoint = {
        .x = player->posX,
        .y = player->posY
    };


    int hitWall     = 0;
    float tempAngle = offsetAngle;
    float tempDirX  = cos(tempAngle);
    float tempDirY  = sin(tempAngle);

    while(hitWall != 1)
    {
        // incrementing ray length
        rayPoint.x += tempDirX;
        rayPoint.y += tempDirY;

        // calculate wall position on the map
        int calcX = rayPoint.x / WALL_SIZE;
        int calcY = rayPoint.y / WALL_SIZE;

        // check if hitting a wall
        if(map[calcX][calcY] == 1){
            hitWall = 1;
        }
    }

    // calculate distance of the wall from the player
    float distance = sqrtf(powf(rayPoint.x - player->posX, 2) + powf(rayPoint.y - player->posY, 2));

    RLRay rayon = {
        .point = rayPoint,
        .distance = distance
    };

    return rayon;
}


// calculate the rays for an angle, as a flashlight
ArrayOfRay* castFlashLight(Player* player, uint8_t map[MAP_SIZE][MAP_SIZE]){
    
    float startAngle = player->angle - (player->fov / 2.0);
    float angleStep  = player->fov / NUMBER_OF_RAYS;

    // array containing the rays
    RLRay* arrayOfRays    = (RLRay*)calloc(NUMBER_OF_RAYS, sizeof(RLRay));
    // struct that hold data of rays
    ArrayOfRay* dataArray = (ArrayOfRay*)calloc(1, sizeof(ArrayOfRay));

    if(arrayOfRays != NULL){
        for (int i = 0; i < NUMBER_OF_RAYS; i++) 
        {
            float rayAngle = startAngle + i * angleStep;
            RLRay calculatedRay = castRay(player, map, rayAngle);
            arrayOfRays[i] = calculatedRay;
        }
    }

    if(dataArray != NULL){
        dataArray->rayList = arrayOfRays;
        dataArray->numberOfRays = NUMBER_OF_RAYS;
    }

    return dataArray;
}


void drawRay(Player* player, Vector2* rayPoint){
    DrawLine(player->posX, player->posY, rayPoint->x, rayPoint->y, GREEN);
}


void drawFlashLight(Player* player, ArrayOfRay* tabPoint){
    for (size_t i = 0; i < tabPoint->numberOfRays; i++)
    {
        drawRay(player, &tabPoint->rayList[i].point);
    }
}
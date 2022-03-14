#ifndef COMP4985_GAME_PROTOTYPE_V0_BULLETS_H
#define COMP4985_GAME_PROTOTYPE_V0_BULLETS_H

#include <stdbool.h>

typedef struct Bullet{
    int damage;
    float xPos;
    float yPos;
    float xSpeed;
    float ySpeed;
    bool live;
} Bullet;

#endif //COMP4985_GAME_PROTOTYPE_V0_BULLETS_H

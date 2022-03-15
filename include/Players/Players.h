#ifndef COMP4985_GAME_PROTOTYPE_V0_PLAYERS_H
#define COMP4985_GAME_PROTOTYPE_V0_PLAYERS_H

#include "Bullets.h"
#include <linux/joystick.h>


typedef struct Player{
    int score;
    int maxNumOfBullets;
    float xPos;
    float yPos;
    float xSpeed;
    float ySpeed;
    float bulletSpeed;
    Bullet *listOfBullets;

    float scopePosX;
    float scopePosY;
    const Uint8 *keyboardState;
} Player;


Player makeNewPlayer(int startingPosX, int startingPosY);
void updatePlayers(struct Player *players, int numOfPlayers, float elapsed, int** mapArray, int mapWidth, int mapHeight,
 int xOffset, int yOffset, int fd, struct js_event jse);
void renderPlayers(SDL_Renderer **renderer, struct Player *players, int numOfPlayers);
void addNewBullet(struct Player player, int posBulletX, int posBulletY);



#endif //COMP4985_GAME_PROTOTYPE_V0_PLAYERS_H

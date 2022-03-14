#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include "Players.h"
#include "Bullets.h"

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 35;
const int PLAYER_MARGIN = 10;
const float PLAYER_SPEED_FOR_BOTH_X_AND_Y = 350.0f;
const float PLAYER_BULLET_SPEED = 800.0f;
const int BULLET_SIZE = 10;
int MAX_NUMBER_OF_BULLETS = 128;


Player makeNewPlayer(int startingPosX, int startingPosY) {
    Player p = {
            .score = 0,
            .maxNumOfBullets = 128,
            .xPos = (float) startingPosX,
            .yPos = (float) startingPosY,
            .xSpeed = PLAYER_SPEED_FOR_BOTH_X_AND_Y,
            .ySpeed = PLAYER_SPEED_FOR_BOTH_X_AND_Y,
            .bulletSpeed = PLAYER_BULLET_SPEED,
            .keyboardState = SDL_GetKeyboardState(NULL),
            .listOfBullets = (Bullet*) malloc(MAX_NUMBER_OF_BULLETS * sizeof(Bullet)),

    };
    for (int i = 0; i < MAX_NUMBER_OF_BULLETS; ++i) {
        p.listOfBullets[i].live = false;
    }
    return p;
}

void updatePlayers(struct Player *players, int numOfPlayers, float elapsed, int** mapArray, int mapWidth, int mapHeight
        , int xOffset, int yOffset) {
    //Error Check
    if (numOfPlayers < 1) {
        fprintf(stderr, "There must be at least 1 player in the game to start.");
        exit(1);
    }

    //Update for each player
    for (int i = 0; i < numOfPlayers; ++i) {
        //Move Up
        if (players[i].keyboardState[SDL_SCANCODE_W] && players[i].yPos > (float) yOffset + (float) PLAYER_HEIGHT / 2)
        {
            players[i].yPos -= players[i].ySpeed * elapsed;
        }

        //Move Down
        if (players[i].keyboardState[SDL_SCANCODE_S] && players[i].yPos < (float) (yOffset + mapHeight - PLAYER_HEIGHT))
        {
            players[i].yPos += players[i].ySpeed  * elapsed;
        }

        //Move Left
        if (players[i].keyboardState[SDL_SCANCODE_A] && players[i].xPos > (float) xOffset+ (float) PLAYER_HEIGHT / 2)
        {
            players[i].xPos -= players[i].xSpeed * elapsed;
        }

        //Move Right
        if (players[i].keyboardState[SDL_SCANCODE_D] && players[i].xPos < (float) (xOffset + mapWidth - PLAYER_WIDTH)) {
            players[i].xPos += players[i].xSpeed  * elapsed;
        }


        //Update all Bullets Position
        for (int j = 0; j < MAX_NUMBER_OF_BULLETS; ++j) {
            if (players[i].listOfBullets[j].live == true) {
                players[i].listOfBullets[j].xPos += players[i].listOfBullets[j].xSpeed * elapsed;
                players[i].listOfBullets[j].yPos += players[i].listOfBullets[j].ySpeed * elapsed;

                if ((int) players[i].listOfBullets[j].xPos < xOffset || (int) players[i].listOfBullets[j].xPos > (xOffset + mapWidth)
                    || (int) players[i].listOfBullets[j].yPos < yOffset || (int) players[i].listOfBullets[j].yPos > (yOffset + mapHeight)) {
                    players[i].listOfBullets[j].live = false;
                }
            }
        }
    }

}

void renderPlayers(SDL_Renderer **renderer, struct Player *players, int numOfPlayers) {
    //Error Check
    if (numOfPlayers < 1) {
        fprintf(stderr, "There must be at least 1 player in the game to render.");
        exit(1);
    }

    for (int i = 0; i < numOfPlayers; ++i) {
        SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 255);
        SDL_Rect currentPlayer = {
                .x = (int)players[i].xPos - PLAYER_HEIGHT / 2,
                .y = (int)players[i].yPos - PLAYER_HEIGHT / 2,
                .w = PLAYER_WIDTH,
                .h = PLAYER_HEIGHT
        };
        SDL_RenderFillRect(*renderer, &currentPlayer);

        //Render Each Bullet
        for (int j = 0; j < MAX_NUMBER_OF_BULLETS; ++j) {
            if (players[i].listOfBullets[j].live == true) {
                SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
                SDL_Rect currentBullet = {
                        .x = (int) players[i].listOfBullets[j].xPos,
                        .y = (int)players[i].listOfBullets[j].yPos,
                        .w = BULLET_SIZE,
                        .h = BULLET_SIZE
                };
                SDL_RenderFillRect(*renderer, &currentBullet);
            }
        }
    }
}

void addNewBullet(struct Player player, int posBulletX, int posBulletY) {
    for (int i = 0; i < MAX_NUMBER_OF_BULLETS; ++i) {
        if (player.listOfBullets[i].live == false) {
            player.listOfBullets[i].live = true;
            player.listOfBullets[i].xPos = player.xPos;
            player.listOfBullets[i].yPos = player.yPos;

            float totalDistance = sqrtf(((player.xPos - (float) posBulletX) * (player.xPos - (float) posBulletX))
                                         + ((player.yPos - (float) posBulletY) * (player.yPos - (float) posBulletY)));
            float speedFactor = totalDistance / player.bulletSpeed;
            player.listOfBullets[i].xSpeed =  ((float)posBulletX - player.xPos) / speedFactor;
            player.listOfBullets[i].ySpeed =  ((float) posBulletY - player.yPos) / speedFactor;
            return;
        }
    }
}










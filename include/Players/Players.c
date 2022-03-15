#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include "Players.h"
#include "Bullets.h"
#include <unistd.h>


const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 35;
const int PLAYER_MARGIN = 10;
const float PLAYER_SPEED_FOR_BOTH_X_AND_Y = 850.0f;
const float PLAYER_BULLET_SPEED = 800.0f;
const int BULLET_SIZE = 10;
int MAX_NUMBER_OF_BULLETS = 128;
const int AXIS_VALUE_LIMIT_MAX = 1000;
const int AXIS_VALUE_LIMIT_MIN = -1000;


void process_event(struct js_event jse, int fd) {

    struct timespec ts;
    uint16_t delay = 50;
    ts.tv_sec = 0;
    ts.tv_nsec = delay * 10000000;


    if (jse.type == 2) {
        if (jse.number == 7) {
            if (jse.value < 0) {
                printf("D-Up\n");
//                nanosleep(&ts, NULL);
                read(fd, &jse, sizeof(jse));
                if (jse.value != 0) {
                    while (1) {
                        read(fd, &jse, sizeof(jse));
                        if (jse.value == 0) {
                            break;
                        }
                        printf("D-Up\n");
                    }
                }
            }
            else if (jse.value > 0) {
                printf("D-Down\n");
            }
        }
        else if (jse.number == 6) {
            if (jse.value < 0) {
                printf("D-Left\n");
            } else if (jse.value > 0) {
                printf("D-Right\n");
            }
        }
        else if (jse.number == 5 && jse.value == -32767) {
            printf("Right Trigger\n");
        }

        else if (jse.number == 4) {
            if (jse.value < -30000) {
                printf("Right-JoyStick Up\n");
            } else if (jse.value > 30000) {
                printf("Right-JoyStick Down\n");
            }
        }

        else if (jse.number == 3) {
            if (jse.value < -30000) {
                printf("Right-Joystick Left\n");
            } else if (jse.value > 30000) {
                printf("Right-Joystick Right\n");
            }
        }

        else if (jse.number == 2 && jse.value == -32767) {
            printf("Left Trigger\n");
        }
        else if (jse.number == 1) {
            if (jse.value < -30000) {
                printf("Left-Joystick Up\n");
            } else if (jse.value > 30000) {
                printf("Left-Joystick Down\n");
            }
        }

        else if (jse.number == 0) {
            if (jse.value < -30000) {
                printf("Left-Joystick Left\n");
            } else if (jse.value > 30000) {
                printf("Left-Joystick Right\n");
            }
        }

    }
    if (jse.type == 1 && jse.value > 0) {
        printf("%d\n", jse.number);
    }


//    printf("type: %d\t", jse.type);
//    printf("number: %d\t", jse.number);
//    printf("value: %d\n", jse.value);



}

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
            .scopePosX = (float) startingPosX,
            .scopePosY = (float) startingPosY,
    };
    for (int i = 0; i < MAX_NUMBER_OF_BULLETS; ++i) {
        p.listOfBullets[i].live = false;
    }
    return p;
}

void updatePlayers(struct Player *players, int numOfPlayers, float elapsed, int** mapArray, int mapWidth, int mapHeight
        , int xOffset, int yOffset, int fd, struct js_event jse) {
    //Error Check
    if (numOfPlayers < 1) {
        fprintf(stderr, "There must be at least 1 player in the game to start.");
        exit(1);
    }

    read(fd, &jse, sizeof(jse));
//    process_event(jse, fd);
    //Update for each player
    for (int i = 0; i < numOfPlayers; ++i) {
        //Player Move Up
//        if (players[i].keyboardState[SDL_SCANCODE_W] && players[i].yPos > (float) yOffset + (float) PLAYER_HEIGHT / 2)
//        {
//            players[i].yPos -= players[i].ySpeed * elapsed;
//        }
        if (jse.type == 2 && jse.number == 1 && jse.value < AXIS_VALUE_LIMIT_MIN && players[i].yPos > (float) yOffset + (float) PLAYER_HEIGHT / 2)
        {
            players[i].yPos -= players[i].ySpeed * elapsed;
        }

        //Player Move Down
        if (jse.type == 2 && jse.number == 1 && jse.value > AXIS_VALUE_LIMIT_MAX && players[i].yPos < (float) (yOffset + mapHeight - PLAYER_HEIGHT))
        {
            players[i].yPos += players[i].ySpeed  * elapsed;
        }

        //Player Move Left
        if (jse.type == 2 && jse.number == 0 && jse.value < AXIS_VALUE_LIMIT_MIN && players[i].xPos > (float) xOffset+ (float) PLAYER_HEIGHT / 2)
        {
            players[i].xPos -= players[i].xSpeed * elapsed;
        }

        //Player Move Right
        if (jse.type == 2 && jse.number == 0 && jse.value > AXIS_VALUE_LIMIT_MAX && players[i].xPos < (float) (xOffset + mapWidth - PLAYER_WIDTH)) {
            players[i].xPos += players[i].xSpeed  * elapsed;
        }

        //Trigger
        if (jse.number == 5 && jse.value == -32767) {
            addNewBullet(players[i], (int) players[i].scopePosX, (int) players[i].scopePosY);
        }

        //Scope Move Up
        if (jse.type == 2 && jse.number == 4 && jse.value < AXIS_VALUE_LIMIT_MIN && players[i].scopePosY > (float) yOffset + (float) 10 / 2) {
            players[i].scopePosY -= players[i].ySpeed * elapsed;
        }

        //Scope Move Down
        if (jse.type == 2 && jse.number == 4 && jse.value > AXIS_VALUE_LIMIT_MAX && players[i].scopePosY < (float) (yOffset + mapHeight - 10)) {
            players[i].scopePosY += players[i].ySpeed  * elapsed;
        }

        //Scope Move Left
        if (jse.type == 2 && jse.number == 3 && jse.value < AXIS_VALUE_LIMIT_MIN && players[i].scopePosX > (float) xOffset + (float) 10 / 2) {
            players[i].scopePosX -= players[i].xSpeed * elapsed;
        }

        //Scope Move Right
        if (jse.type == 2 && jse.number == 3 && jse.value > AXIS_VALUE_LIMIT_MAX && players[i].scopePosX < (float) (xOffset + mapWidth - 10)) {
            players[i].scopePosX += players[i].xSpeed * elapsed;
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
        //Render Player
        SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 255);
        SDL_Rect currentPlayer = {
                .x = (int)players[i].xPos - PLAYER_HEIGHT / 2,
                .y = (int)players[i].yPos - PLAYER_HEIGHT / 2,
                .w = PLAYER_WIDTH,
                .h = PLAYER_HEIGHT
        };
        SDL_RenderFillRect(*renderer, &currentPlayer);

        //Render Scope
        SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
        SDL_Rect rectScope = {
                .x = (int)players[i].scopePosX,
                .y = (int)players[i].scopePosY,
                .w = 10,
                .h = 10
        };
        SDL_RenderFillRect(*renderer, &rectScope);

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










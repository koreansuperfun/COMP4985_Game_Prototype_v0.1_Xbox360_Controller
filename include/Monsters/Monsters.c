#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Monsters.h"

float MONSTER_LIFE_DURATION = 1.5f;
const float MONSTER_HEIGHT = 70;
const float MONSTER_WIDTH = 40;

int getRandomNumberGenerator(int lower, int upper) {
    srand(time(0));
    return (rand() % (upper - lower + 1) + lower);

}
void randomlyGenerateMonster(Monster *arrayOfMonster, int chanceOfSpawn, int numOfMonsters,
                             int xMinPos, int xMaxPos,
                             int yMinPos, int yMaxPos) {
    if (chanceOfSpawn < 0 || chanceOfSpawn > 100) {
        fprintf(stderr, "chanceOfSpawn must be between 0 and 100\n");
    }


    int chance = getRandomNumberGenerator(0, 100);
    if (chance < chanceOfSpawn) {
        for (int i = 0; i < numOfMonsters; ++i) {
            if (arrayOfMonster[i].alive == false) {
                arrayOfMonster[i].alive = true;
                arrayOfMonster[i].xPos = (float) getRandomNumberGenerator(xMinPos, xMaxPos);
                arrayOfMonster[i].yPos = (float) getRandomNumberGenerator(yMinPos, yMaxPos);
                arrayOfMonster[i].currentDuration = 0;
                return;

            }
        }
    }
}

Monster* createArrayOfMonsters(int numOfMonsters) {
    Monster *monsters = (Monster*) malloc(numOfMonsters * sizeof(Monster));
    for (int i = 0; i < numOfMonsters; ++i) {
        monsters[i].xPos = 0;
        monsters[i].yPos = 0;
        monsters[i].width = MONSTER_WIDTH;
        monsters[i].height = MONSTER_HEIGHT;
        monsters[i].lifeDuration = MONSTER_LIFE_DURATION;
        monsters[i].currentDuration = 0;
        monsters[i].alive = false;
    }
    return monsters;
}

void updateMonstersLifeDuration(float elapsed, struct Monster *arrayOfMonsters,
                                int numOfMonsters) {
    for (int i = 0; i < numOfMonsters; ++i) {
        if (arrayOfMonsters[i].alive == true) {
            arrayOfMonsters[i].currentDuration += elapsed;
            if (arrayOfMonsters[i].currentDuration > arrayOfMonsters[i].lifeDuration) {
                arrayOfMonsters[i].alive = false;
                arrayOfMonsters[i].currentDuration = 0;
            }
        }
    }
}

void renderMonsters(SDL_Renderer **renderer, struct Monster *monsters, int numOfMonsters) {
    for (int i = 0; i < numOfMonsters; ++i){
        if (monsters[i].alive == true) {
            SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
            SDL_Rect currentMonster = {
                    .x = (int)monsters[i].xPos,
                    .y = (int)monsters[i].yPos,
                    .w = (int)monsters[i].width,
                    .h = (int)monsters[i].height,
            };
            SDL_RenderFillRect(*renderer, &currentMonster);
        }
    }
}














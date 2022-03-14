#ifndef COMP4985_GAME_PROTOTYPE_V0_MONSTERS_H
#define COMP4985_GAME_PROTOTYPE_V0_MONSTERS_H

#include <stdbool.h>

typedef struct Monster{
    float xPos;
    float yPos;
    float width;
    float height;
    float lifeDuration;
    float currentDuration;
    bool alive;
} Monster;

Monster* createArrayOfMonsters(int numOfMonsters);
void randomlyGenerateMonster(Monster *arrayOfMonster, int chanceOfSpawn, int numOfMonsters,
                             int xMinPos, int xMaxPos,
                             int yMinPos, int yMaxPos);
void updateMonstersLifeDuration(float elapsed, Monster *arrayOfMonsters,
                                int numOfMonsters);
void renderMonsters(SDL_Renderer **renderer, struct Monster *monsters, int numOfMonsters);

#endif //COMP4985_GAME_PROTOTYPE_V0_MONSTERS_H

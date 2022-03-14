#ifndef COMP4985_GAME_PROTOTYPE_V0_MAPS_H
#define COMP4985_GAME_PROTOTYPE_V0_MAPS_H

typedef struct Maps{
    int height;
    int width;
    int** map_array;
    int startingPosX;
    int startingPosY;
} Maps;

Maps makeMap(int height, int width, int** mapArray);
void renderMap(SDL_Renderer **renderer, Maps maps, int screenHeight, int screenWidth);
void setBorderLine(Maps *maps, int startingX, int startingY, int distance, char dir);


#endif //COMP4985_GAME_PROTOTYPE_V0_MAPS_H

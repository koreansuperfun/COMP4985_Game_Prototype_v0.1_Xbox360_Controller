#include <SDL2/SDL.h>
#include "Maps.h"

const int BORDER_THICKNESS = 5;
const int BORDER_VALUE_IN_ARRAY = 1;

Maps makeMap(int height, int width, int** mapArray) {
    Maps m = {
      .height = height,
      .width = width,
      .map_array = mapArray,
      .startingPosX = width / 2,
      .startingPosY = height / 2,
    };
    return m;
}

void setBorderLine(Maps *maps, int startingX, int startingY, int distance, char dir) {
    if (startingX < 0 || startingY < 0 || startingX > maps->width || startingY > maps->height) {
        fprintf(stderr, "Invalid staring position. Out of bounds from map.");
    }

    if (distance <= 0) {
        fprintf(stderr, "Distance must be greater than 0.");
    }

    int counter = 0;

    if (dir == 'l') {
        for (int x = startingX; (x >= 0) && (counter < distance); --x) {
            maps->map_array[startingY][x] = BORDER_VALUE_IN_ARRAY;
            ++counter;
        }

    } else if (dir == 'r') {
        for (int x = startingX; (x < maps->width) && (counter < distance); ++x) {
            maps->map_array[startingY][x] = BORDER_VALUE_IN_ARRAY;
            ++counter;
        }
    } else if (dir == 'u') {
        for (int y = startingY; (y >= 0) && (counter < distance); --y) {
            maps->map_array[y][startingX] = BORDER_VALUE_IN_ARRAY;
            ++counter;
        }
    } else if (dir == 'd') {
        for (int y = startingY; (y < maps->height) && (counter < distance); ++y) {
            maps->map_array[y][startingX] = BORDER_VALUE_IN_ARRAY;
            ++counter;
        }
    } else {
        fprintf(stderr, "Direction must be a char of either u, d, l , or r.");
    }
}


void renderMap(SDL_Renderer **renderer, Maps maps, int screenHeight, int screenWidth) {
    int xOffSetToCenter = (screenWidth / 2) - (maps.width / 2);
    int yOffSetToCenter = (screenHeight / 2) - (maps.height / 2);

    SDL_SetRenderDrawColor(*renderer, 180, 180, 180, 1);
    SDL_Rect playable_area = {
            .x = xOffSetToCenter,
            .y = yOffSetToCenter,
            .w = maps.width,
            .h = maps.height,
    };
    SDL_RenderFillRect(*renderer, &playable_area);

    for (int y = 0; y < maps.height; ++y) {
        for (int x = 0; x < maps.width; ++x) {
            if (maps.map_array[y][x] == 1) {
                SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
                SDL_Rect border_block = {
                        .x = xOffSetToCenter + x,
                        .y = yOffSetToCenter + y,
                        .w = BORDER_THICKNESS,
                        .h = BORDER_THICKNESS
                };
                SDL_RenderFillRect(*renderer, &border_block);
            }
        }
    }
}







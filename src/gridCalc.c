#include "../include/gridCalc.h"
//#define NDEBUG
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>


void setNeighbours(int** neighbours, int* middleTile, int arrayPosX, int arrayPosY);

void setNextGeneration(int* tiles) {

    //Copy old tiles to working array
    int oldTiles[AMOUNT_TILES_LANE][AMOUNT_TILES_LANE];

    for (int i = 0; i < AMOUNT_TILES_LANE; i++) {
        for (int k = 0; k < AMOUNT_TILES_LANE; k++) {
            if(tiles[i*AMOUNT_TILES_LANE + k] == 1) {
                tiles[i*AMOUNT_TILES_LANE + k] = 0;
                oldTiles[i][k] = 1;
            } else {
                oldTiles[i][k] = 0;
            }
        }
    }


    int* neighbours[3][3];
    for (int i = 0; i < AMOUNT_TILES_LANE; i++) {
        for (int k = 0; k < AMOUNT_TILES_LANE; k++) {    
            if (oldTiles[i][k] == 1) {
                setNeighbours(&neighbours[0][0],&oldTiles[i][k],k,i);
            }
        }
    }

}

/// @brief Sets the pointer to the neighbours in a 3x3 grid
/// @param neighbours Array to be filled with neighbours
/// @param middleTile Middle tile of old generation for gathering of its neighbours
/// @param arrayPosX X position of middle tile in array
/// @param arrayPosY Y position of middle tile in array
void setNeighbours(int** neighbours, int* middleTile, int arrayPosX, int arrayPosY) {
    int* firstTile = middleTile-(arrayPosY * AMOUNT_TILES_LANE + arrayPosX);
    neighbours[5] = middleTile;

    //ABOVE
    if (arrayPosY == 0) {
        neighbours[1] = middleTile+(AMOUNT_TILES_LANE*(AMOUNT_TILES_LANE-1));
    } else {
        neighbours[1] = middleTile-(AMOUNT_TILES_LANE);
    }
    //LEFT
    if (arrayPosX == 0) {
        neighbours[3] = middleTile+(AMOUNT_TILES_LANE-1);
    } else {
        neighbours[3] = middleTile-1;
    }
    //DOWN
    if (arrayPosY == AMOUNT_TILES_LANE-1) {
        neighbours[8] = middleTile-(AMOUNT_TILES_LANE*(AMOUNT_TILES_LANE-1));
    } else {
        neighbours[8] = middleTile+(AMOUNT_TILES_LANE);
    }
    //RIGHT
    if (arrayPosX == AMOUNT_TILES_LANE-1) {
        neighbours[5] = middleTile-(AMOUNT_TILES_LANE-1);
    } else {
        neighbours[5] = middleTile+1;
    }



/*     if ((arrayPosX == 0) && (arrayPosY == 0)) {
        neighbours[0] = firstTile+(AMOUNT_TILES_LANE*AMOUNT_TILES_LANE-1);
    } else if (arrayPosX == 0) {
        neighbours[0] = middleTile-1;
    } else if (arrayPosY == 0) {
        neighbours[0] = firstTile+(AMOUNT_TILES_LANE*(AMOUNT_TILES_LANE-1)+arrayPosX-1);
    } else {

    } */

    

    
}



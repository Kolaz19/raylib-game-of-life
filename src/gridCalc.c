#include "../include/gridCalc.h"
//#define NDEBUG
#include <assert.h>
#include <stdbool.h>


void setNeighbours(int** neighbours, int* middleTile, int middleTilePos);

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
                setNeighbours(&neighbours[0][0],&oldTiles[i][k],i*AMOUNT_TILES_LANE + k);
            }
        }
    }

}

/// @brief Sets the pointer to the neighbours in a 3x3 grid
/// @param neighbours Array to be filled with neighbours
/// @param middleTile Middle tile of old generation for gathering of its neighbours
/// @param middleTilePos Position in array of middle tile
void setNeighbours(int** neighbours, int* middleTile, int middleTilePos) {
    neighbours[5] = middleTile;
}



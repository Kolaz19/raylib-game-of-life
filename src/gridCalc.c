#include "../include/gridCalc.h"
#include "../include/gridProperties.h"
//#define NDEBUG
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>


void setNeighbours(int** neighbours, int* middleCell, int arrayPosX, int arrayPosY);
int getNumberOfNeighbours(int **neighbours);
bool willCellLive(int **neighbours);

void setNextGeneration(int* cells) {

    //Copy current cells to working array and clear current cells
    int oldCells[AMOUNT_CELLS_LANE][AMOUNT_CELLS_LANE];

    for (int i = 0; i < AMOUNT_CELLS_LANE; i++) {
        for (int k = 0; k < AMOUNT_CELLS_LANE; k++) {
            if(cells[i*AMOUNT_CELLS_LANE + k] == 1) {
                cells[i*AMOUNT_CELLS_LANE + k] = 0;
                oldCells[i][k] = 1;
            } else {
                oldCells[i][k] = 0;
            }
        }
    }


    int* neighbours[3][3];
    for (int i = 0; i < AMOUNT_CELLS_LANE; i++) {
        for (int k = 0; k < AMOUNT_CELLS_LANE; k++) {    

            setNeighbours(&neighbours[0][0],&oldCells[i][k],k,i);
            //Activate cell
            if (willCellLive(&neighbours[0][0])) {
                cells[i*AMOUNT_CELLS_LANE + k] = 1;
            }
        }
    }
}



/// @brief Sets the pointer to the neighbours in a 3x3 grid
/// @param neighbours Array to be filled with neighbours
/// @param middleCell Middle cell of old generation for gathering of its neighbours
/// @param arrayPosX X position of middle cell in array
/// @param arrayPosY Y position of middle cell in array
void setNeighbours(int** neighbours, int* middleCell, int arrayPosX, int arrayPosY) {
    neighbours[4] = middleCell;

    //ABOVE
    if (arrayPosY == 0) {
        neighbours[1] = middleCell+(AMOUNT_CELLS_LANE*(AMOUNT_CELLS_LANE-1));
    } else {
        neighbours[1] = middleCell-(AMOUNT_CELLS_LANE);
    }
    //LEFT
    if (arrayPosX == 0) {
        neighbours[3] = middleCell+(AMOUNT_CELLS_LANE-1);
    } else {
        neighbours[3] = middleCell-1;
    }
    //DOWN
    if (arrayPosY == AMOUNT_CELLS_LANE-1) {
        neighbours[7] = middleCell-(AMOUNT_CELLS_LANE*(AMOUNT_CELLS_LANE-1));
    } else {
        neighbours[7] = middleCell+(AMOUNT_CELLS_LANE);
    }
    //RIGHT
    if (arrayPosX == AMOUNT_CELLS_LANE-1) {
        neighbours[5] = middleCell-(AMOUNT_CELLS_LANE-1);
    } else {
        neighbours[5] = middleCell+1;
    }

    //ABOVE LEFT
    if(arrayPosX == 0) {
        neighbours[0] = neighbours[1]+(AMOUNT_CELLS_LANE-1);
    } else {
        neighbours[0] = neighbours[1]-1;
    }
    //DOWN LEFT
    if(arrayPosX == 0) {
        neighbours[6] = neighbours[7]+(AMOUNT_CELLS_LANE-1);
    } else {
        neighbours[6] = neighbours[7]-1;    
    }
    //ABOVE RIGHT
    if(arrayPosX == AMOUNT_CELLS_LANE-1) {
        neighbours[2] = neighbours[1]-(AMOUNT_CELLS_LANE-1);
    } else {
        neighbours[2] = neighbours[1]+1;
    }
    //DOWN RIGHT
    if(arrayPosX == AMOUNT_CELLS_LANE-1) {
        neighbours[8] = neighbours[7]-(AMOUNT_CELLS_LANE-1);
    } else {
        neighbours[8] = neighbours[7]+1;
    }

//Test neighbour layout
/*     printf("%d-%d-%d\n", *neighbours[0],*neighbours[1], *neighbours[2]);
    printf("%d-%d-%d\n", *neighbours[3],*neighbours[4], *neighbours[5]);
    printf("%d-%d-%d\n\n", *neighbours[6],*neighbours[7], *neighbours[8]); */

}


bool willCellLive(int **neighbours) {
    int cellNeighbourCount = getNumberOfNeighbours(neighbours);
    //If middle cell is dead
    if (*neighbours[4] == 0) {
        if (cellNeighbourCount == 3) {
            return true;
        }
    //If middle cell is alive
    } else {
        if (cellNeighbourCount == 2 || cellNeighbourCount == 3) {
            return true;
        }
    }
    return false;
}

int getNumberOfNeighbours(int **neighbours) {
    int numberOfNeighbours = 0;
    for (int i = 0; i < 9; i++) {
        //Ignore middle cell
        if (i == 4) {
            continue;
        }

        if (*neighbours[i] == 1) {
            numberOfNeighbours++;
        }
    }
    return numberOfNeighbours;
}
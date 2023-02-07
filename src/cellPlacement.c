#include "../include/cellPlacement.h"
#include "../include/gridProperties.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum EditMode {
    NONE,
    PLACE,
    REMOVE
} EditMode;

EditMode getEditMode();
int* getCellAtMousePos(int *cells, Vector2 mousePos);


void placeCellWithCursor(int *cells, Camera2D *cam) {
    EditMode editMode = getEditMode();
    if (editMode == NONE) {
        return;
    }

    int *cellAtMouse = getCellAtMousePos(cells,GetScreenToWorld2D(GetMousePosition(),*cam));

    if (cellAtMouse == NULL) {
        return;
    }

    if (editMode == PLACE) {
        *cellAtMouse = 1;
    }
}


EditMode getEditMode() {
    bool isLeftMouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool isRightMouseDown = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

    if (isLeftMouseDown && !isRightMouseDown) {
        return PLACE;
    } else if(isRightMouseDown && !isLeftMouseDown) {
        return REMOVE;
    } else {
        return NONE;
    }
}

int* getCellAtMousePos(int *cells, Vector2 mousePos) {
    int xCell = (int)mousePos.x / SPACE_BETWEEN_CELLS;
    int yCell = (int)mousePos.y / SPACE_BETWEEN_CELLS;
    Rectangle rect = {xCell*SPACE_BETWEEN_CELLS+1,yCell*SPACE_BETWEEN_CELLS+1,SPACE_BETWEEN_CELLS-1,SPACE_BETWEEN_CELLS-1};
    if(!CheckCollisionPointRec(mousePos,rect)) {
        return NULL;
    }
    return cells+(yCell*AMOUNT_CELLS_LANE+xCell);
}
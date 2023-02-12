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
bool isMouseOutOfBounds(Vector2 *mousePos);


void placeCellWithCursor(int *cells, Camera2D *cam) {
    EditMode editMode = getEditMode();
    if (editMode == NONE) return;

    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(),*cam);
    if (isMouseOutOfBounds(&mousePos)) return;

    int *cellAtMouse = getCellAtMousePos(cells,mousePos);
    if (cellAtMouse == NULL) return;

    if (editMode == PLACE) {
        *cellAtMouse = 1;
    } else {
        *cellAtMouse = 0;
    }
}

/// @brief Check if right or left mouse is pressed
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

/// @brief Get the cell at the mouse position
/// @param mousePos Mouse world position
/// @return Return cell or NULL if the cursor is between two cells
int* getCellAtMousePos(int *cells, Vector2 mousePos) {
    //Get starting point of cell to evaluate (because rounded down)
    int xCell = (int)mousePos.x / SPACE_BETWEEN_CELLS;
    int yCell = (int)mousePos.y / SPACE_BETWEEN_CELLS;
    Rectangle rect = {xCell*SPACE_BETWEEN_CELLS+1,yCell*SPACE_BETWEEN_CELLS+1,SPACE_BETWEEN_CELLS-1,SPACE_BETWEEN_CELLS-1};
    if(!CheckCollisionPointRec(mousePos,rect)) return NULL;
    
    return cells+(yCell*AMOUNT_CELLS_LANE+xCell);
}

bool isMouseOutOfBounds(Vector2 *mousePos) {
    if ((mousePos->x > AMOUNT_CELLS_LANE * SPACE_BETWEEN_CELLS)
    || (mousePos->y > AMOUNT_CELLS_LANE * SPACE_BETWEEN_CELLS)) {
        return true;
    } else {
        return false;
    }
}
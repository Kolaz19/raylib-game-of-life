#include "../include/cellPlacement.h"
#include "../include/gridProperties.h"
#include <stdbool.h>

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
    
}
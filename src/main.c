#include "../include/raylib.h"
#include "../include/gridCalc.h"
#include "../include/cellPlacement.h"
#include "../include/gridProperties.h"
#include <stdio.h>
#include <stdbool.h>


typedef enum GameState {
    PLACE_TILES,
    SIMULATE
} GameState;

/// @brief Is used to display rectangles at side of screen to
/// indicate that cam collides with border
typedef struct CamBorderCollision {
    bool left;
    bool right;
    bool up;
    bool down;
} CamBorderCollision;

const int screenWidth = 1920*0.8f;
const int screenHeight = 1080*0.8f;
GameState currentGameState = PLACE_TILES;



void prepareBackgroundTexture(RenderTexture2D *texture);
void zoomCam(Camera2D *cam, CamBorderCollision *camBorderCollision);
void moveCam(Camera2D *cam, CamBorderCollision *camBorderCollision);
void adjustTargetToMouse(Camera2D *cam);
bool isCamOutOfBounds(Camera2D *cam);
void simulate(int *cells, int *frameCounter, int frameLimitForUpdate);
void checkSwitchModes(void);
void checkResetState(int *cells, Camera2D *cam);
void drawUI(CamBorderCollision *camBorderCollision, int *frameLimitForUpdate);
void checkForUpdateSpeed(int *frameLimitForUpdate, int *frameCounter);

int main(void) {
    //Initialization
    InitWindow(screenWidth, screenHeight, "Game of Life");
    Image icon = LoadImage("game_of_life_icon.png");
    SetWindowIcon(icon);
    //Background
    RenderTexture2D backgroundTexture = LoadRenderTexture(BACKGROUND_SIZE,BACKGROUND_SIZE);
    prepareBackgroundTexture(&backgroundTexture);
    //Camera
    Camera2D cam = {0};
    cam.zoom = 3.0f;
    cam.target.x = (BACKGROUND_SIZE / 2) - (screenWidth / cam.zoom / 2);
    cam.target.y = (BACKGROUND_SIZE / 2) - (screenHeight / cam.zoom / 2);
    CamBorderCollision camBorderCollision = {false};
    //Tiles
    int cells[AMOUNT_CELLS_LANE][AMOUNT_CELLS_LANE] = {0};
    //Simulation Update Cycle
    int frameCounter = 0;
    int frameLimitForUpdate = 30;
    SetTargetFPS(60);              


    //Main game loop
    while (!WindowShouldClose()) {

        //Reset cam border collision
        camBorderCollision.down = false;
        camBorderCollision.up = false;
        camBorderCollision.right = false;
        camBorderCollision.left = false;

        //Update cam and check if border is hit with zoom or movement
        zoomCam(&cam, &camBorderCollision);
        moveCam(&cam, &camBorderCollision);
        //Update
        checkSwitchModes();
        checkForUpdateSpeed(&frameLimitForUpdate, &frameCounter);
        checkResetState(&cells[0][0],&cam);
        switch(currentGameState) {
            case SIMULATE:
                simulate(&cells[0][0],&frameCounter,frameLimitForUpdate);
                break;
        }
        placeCellWithCursor(&cells[0][0],&cam);
        


        //Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode2D(cam);
            DrawTexture(backgroundTexture.texture,0,0,WHITE);
            for(int i = 0; i < AMOUNT_CELLS_LANE; i++) {
                for (int k = 0; k < AMOUNT_CELLS_LANE; k++) {
                    if(cells[i][k] == 1) {
                        DrawRectangle(k*SPACE_BETWEEN_CELLS+1,i*SPACE_BETWEEN_CELLS+1,SPACE_BETWEEN_CELLS-1,SPACE_BETWEEN_CELLS-1,DARKPURPLE);
                    }
                }
            } 
            EndMode2D();
            drawUI(&camBorderCollision, &frameLimitForUpdate);
        EndDrawing();

    }
    CloseWindow();        
    return 0;
}

/// @brief Draws all lines to one background texture
void prepareBackgroundTexture(RenderTexture2D *texture) {
    BeginTextureMode(*texture);
            DrawRectangle(0,0,BACKGROUND_SIZE,BACKGROUND_SIZE,RAYWHITE);
            for(int i = 0; i < BACKGROUND_SIZE/SPACE_BETWEEN_CELLS+1; i++) {
                DrawLine(i*SPACE_BETWEEN_CELLS+1 ,0 ,i*SPACE_BETWEEN_CELLS+1 ,BACKGROUND_SIZE ,LIGHTGRAY);
                DrawLine(0 ,i*SPACE_BETWEEN_CELLS ,BACKGROUND_SIZE ,i*SPACE_BETWEEN_CELLS ,LIGHTGRAY);
            }                     
    EndTextureMode();
}

/// @brief Zoom to mouse position
void zoomCam(Camera2D *cam, CamBorderCollision *camBorderCollision) {
    const float zoomSpeed = 0.3f;
    float mouseWheelMovement = GetMouseWheelMove();

    if (mouseWheelMovement != 0) {
        //Mouse position should be target for zoom
        adjustTargetToMouse(cam);
    }

    if(mouseWheelMovement > 0) {
        cam->zoom += zoomSpeed;
    } else if(mouseWheelMovement < 0) {
        cam->zoom -= zoomSpeed;
    }

    if(cam->zoom < 1.2f) {
        cam->zoom = 1.2f;
    } else if (cam->zoom > 6.0f) {
        cam->zoom = 6.0f;
    }

    while(isCamOutOfBounds(cam)) {
        cam->zoom += 0.01f;
        camBorderCollision->down = true;
        camBorderCollision->up = true;
        camBorderCollision->left = true;
        camBorderCollision->right = true;
    }

    //Alternative cam (static/without mouse target)
    //cam->offset = (Vector2){(float)GetScreenWidth()/2.0f,(float)GetScreenHeight()/2.0f};
}

void moveCam(Camera2D *cam, CamBorderCollision *camBorderCollision) {
    static Vector2 previousMousePosition = {0};
    //Save previous mose position to calculate drag distance between
    //old and new mouse position
    //For performance reasons this is only saved once for initial click
    if(IsKeyPressed(KEY_SPACE)) {
        previousMousePosition = GetScreenToWorld2D(GetMousePosition(),*cam);
    }

    if(IsKeyDown(KEY_SPACE)) {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(),*cam);
        Vector2 difference = {mouseWorldPos.x - previousMousePosition.x, mouseWorldPos.y - previousMousePosition.y};
        Vector2 oldTarget = cam->target;
        //Set to map boundaries when out of bounds
        //x-achsis adjustment
        cam->target.x = cam->target.x - difference.x;
        if (isCamOutOfBounds(cam)) { 
            if (oldTarget.x > cam->target.x) {
                cam->target.x = 0 + cam->offset.x / cam->zoom;
                camBorderCollision->left = true;
            } else {
                cam->target.x = (float)BACKGROUND_SIZE - ((float)GetScreenWidth() / cam->zoom) + (cam->offset.x / cam->zoom);
                camBorderCollision->right = true;
            }

        }
        //y-achsis adjustment
        cam->target.y = cam->target.y - difference.y;
        if (isCamOutOfBounds(cam)) {
            if (oldTarget.y > cam->target.y) {
                cam->target.y = 0 + cam->offset.y / cam->zoom;
                camBorderCollision->up = true;
            } else {
                cam->target.y = (float)BACKGROUND_SIZE - ((float)GetScreenHeight() / cam->zoom) + (cam->offset.y / cam->zoom);
                camBorderCollision->down = true;
            }
        }
    }
}

/// @brief Set target of camera to mouse position
void adjustTargetToMouse(Camera2D *cam) {
    Vector2 mousePos = GetMousePosition();
    Vector2 mousePosWorld = GetScreenToWorld2D(mousePos,*cam);
    //Also set new offset so camera can stay in place
    Vector2 offset = {GetScreenWidth() - (GetScreenWidth() - mousePos.x), GetScreenHeight() - (GetScreenHeight() - mousePos.y)};
    cam->offset = offset;

    cam->target.x = mousePosWorld.x;
    cam->target.y = mousePosWorld.y;
}
/// @brief Check if camera shows area outside of background
bool isCamOutOfBounds(Camera2D *cam) {
    Vector2 camPosLeftUpper = { cam->target.x - cam->offset.x/ cam->zoom, cam->target.y - cam->offset.y/ cam->zoom};
    Vector2 camPosRightDown = {camPosLeftUpper.x + GetScreenWidth()/cam->zoom, camPosLeftUpper.y + GetScreenHeight()/cam->zoom};
    if (camPosLeftUpper.x < 0 || camPosLeftUpper.y < 0 || camPosRightDown.x > BACKGROUND_SIZE || camPosRightDown.y > BACKGROUND_SIZE) {
        return true;
    } else {
        return false;
    }
} 

/// @brief Update cells in grid after specific period of time
/// @param frameCounter Counts up after every call and resets after frameLimitForUpdate is reached
/// @param frameLimitForUpdate Update after this many frames
void simulate(int *cells, int *frameCounter, int frameLimitForUpdate) {
    if (*frameCounter == frameLimitForUpdate) {
        setNextGeneration(cells);
        *frameCounter = 0;
    } else {
        (*frameCounter)++;
    }  
}

void checkSwitchModes(void) {
    if(!IsKeyPressed(KEY_ENTER)) return;
    
    if(currentGameState == PLACE_TILES) {
        currentGameState = SIMULATE;
    } else {
        currentGameState = PLACE_TILES;
    }
}

void checkResetState(int *cells, Camera2D *cam) {
    if(!IsKeyPressed(KEY_R)) return;

    currentGameState = PLACE_TILES;
     for (int i = 0; i < AMOUNT_CELLS_LANE; i++) {
        for (int k = 0; k < AMOUNT_CELLS_LANE; k++) {
            *(cells+(k*AMOUNT_CELLS_LANE+i)) = 0;
        }
    }
    cam->zoom = 3.0f;
    cam->target.x = (BACKGROUND_SIZE / 2) - (screenWidth / cam->zoom / 2);
    cam->target.y = (BACKGROUND_SIZE / 2) - (screenHeight / cam->zoom / 2);
}

void drawUI(CamBorderCollision *camBorderCollision, int *frameLimitForUpdate) {
    if (currentGameState == PLACE_TILES) {
        //Pause
        DrawRectangle(GetScreenWidth()-50,GetScreenHeight()-90,20,60,BLACK);
        DrawRectangle(GetScreenWidth()-85,GetScreenHeight()-90,20,60,BLACK);
        //Keys
        DrawText("ENTER",30,30,30,RED);
        DrawText("SPACE",30,60,30,RED);
        DrawText("R",30,90,30,RED);
        DrawText("ARROW UP",30,120,30,RED);
        DrawText("ARROW DOWN",30,150,30,RED);
        //Keys Description
        DrawText("PLAY/PAUSE",150,30,30,BLACK);
        DrawText("MOVE CAMERA",150,60,30,BLACK);
        DrawText("RESET",150,90,30,BLACK);
        DrawText("SPEED +",250,120,30,BLACK);
        DrawText("SPEED -",250,150,30,BLACK);
        //Box around key description
        DrawRectangleLinesEx((Rectangle){20,22,380,165},4,BLACK);    
        //Draw Mouse Buttons
        DrawText("LEFT MOUSE",450,30,30,RED);
        DrawText("RIGHT MOUSE",450,60,30,RED);
        DrawText("MOUSE WHEEL",450,90,30,RED); 
        //Draw Mouse Button Description
        DrawText("PLACE CELL",690,30,30,BLACK);
        DrawText("DELETE CELL",690,60,30,BLACK);
        DrawText("ZOOM",690,90,30,BLACK);
        //Box around mouse
        DrawRectangleLinesEx((Rectangle){440,22,475,105},4,BLACK);  

    }
    //Draw cam colliding with border
    if (camBorderCollision->left == true) {
        DrawRectangleGradientV(0,0,10,GetScreenHeight(),YELLOW,PURPLE);
    }
    if (camBorderCollision->up == true) {
        DrawRectangleGradientH(0,0,GetScreenWidth(),10,YELLOW,PURPLE);
    }
    if (camBorderCollision->right == true) {
        DrawRectangleGradientV(GetScreenWidth()-10,0,10,GetScreenHeight(),PURPLE,YELLOW);
    }
    if (camBorderCollision->down == true) {
        DrawRectangleGradientH(0,GetScreenHeight()-10,GetScreenWidth(),10,PURPLE,YELLOW);
    }
    //Draw Updates per second
    int upsToDisplay = 60 / *frameLimitForUpdate;
    char ups[6] = "\0";
    ups[0] = (int)(upsToDisplay/10) + '0';
    if(ups[0] == '0') ups[0] = ' ';
    ups[1] = (upsToDisplay%10) + '0';
    ups[2] = 'U';
    ups[3] = 'P';
    ups[4] = 'S';
    DrawText(ups,GetScreenWidth()-80,10,20,BLACK);
}

void checkForUpdateSpeed(int *frameLimitForUpdate, int *frameCounter) {

    if (IsKeyPressed(KEY_DOWN)) {
        switch (*frameLimitForUpdate) {
            case 1: *frameLimitForUpdate = 2; break;
            case 2: *frameLimitForUpdate = 5; break;
            case 5: *frameLimitForUpdate = 10; break;
            case 10: *frameLimitForUpdate = 20; break;
            case 20: *frameLimitForUpdate = 30; break;
            case 30: *frameLimitForUpdate = 60; break;
        }
    } else if (IsKeyPressed(KEY_UP)) {
         switch (*frameLimitForUpdate) {
            case 60: *frameLimitForUpdate = 30; break;
            case 30: *frameLimitForUpdate = 20; break;
            case 20: *frameLimitForUpdate = 10; break;
            case 10: *frameLimitForUpdate = 5; break;
            case 5: *frameLimitForUpdate = 2; break;
            case 2: *frameLimitForUpdate = 1; break;
        }       
    }

    if (*frameCounter > *frameLimitForUpdate) {
        *frameCounter = *frameLimitForUpdate;
    }
}
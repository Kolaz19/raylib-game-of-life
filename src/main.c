#include "../include/raylib.h"
#include "../include/gridCalc.h"
#include <stdio.h>
#include <stdbool.h>


#define SPACE_BETWEEN_TILES 5
#define BACKGROUND_SIZE (AMOUNT_TILES_LANE*SPACE_BETWEEN_TILES+1)

const int screenWidth = 1920*0.8f;
const int screenHeight = 1080*0.8f;



void prepareBackgroundTexture(RenderTexture2D *texture);
void zoomCam(Camera2D *cam);
void moveCam(Camera2D *cam);
void adjustTargetToMouse(Camera2D *cam);
bool isCamOutOfBounds(Camera2D *cam);

int main(void)
{
    //Initialization
    SetConfigFlags(FLAG_INTERLACED_HINT);
    InitWindow(screenWidth, screenHeight, "Game of Life");
    //Background
    RenderTexture2D backgroundTexture = LoadRenderTexture(BACKGROUND_SIZE,BACKGROUND_SIZE);
    prepareBackgroundTexture(&backgroundTexture);
    //Camera
    Camera2D cam = {0};
    cam.zoom = 3.0f;
    cam.target.x = (BACKGROUND_SIZE / 2) - (screenWidth / cam.zoom / 2);
    cam.target.y = (BACKGROUND_SIZE / 2) - (screenHeight / cam.zoom / 2);
    //Tiles
    int tiles[AMOUNT_TILES_LANE][AMOUNT_TILES_LANE] = {0};
    tiles[0][0] = 1;
    tiles[AMOUNT_TILES_LANE-1][0] = 1;
    tiles[2][6] = 1; 
    tiles[57][0] = 1;


    SetTargetFPS(60);              


    //Main game loop
    while (!WindowShouldClose()) {
    
        //Update
        zoomCam(&cam);
        moveCam(&cam);
        setNextGeneration(&tiles[0][0]);

        //Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode2D(cam);
            DrawTexture(backgroundTexture.texture,0,0,WHITE);
            for(int i = 0; i < AMOUNT_TILES_LANE; i++) {
                for (int k = 0; k < AMOUNT_TILES_LANE; k++) {
                    if(tiles[i][k] == 1) {
                        DrawRectangle(i*5+1,k*5+1,SPACE_BETWEEN_TILES-1,SPACE_BETWEEN_TILES-1,RED);
                    }
            }
            } 
            
            EndMode2D();


        EndDrawing();

    }
    CloseWindow();        
    return 0;
}
/// @brief Draws all lines to one background texture
void prepareBackgroundTexture(RenderTexture2D *texture) {
    BeginTextureMode(*texture);
            DrawRectangle(0,0,BACKGROUND_SIZE,BACKGROUND_SIZE,RAYWHITE);
            for(int i = 0; i < BACKGROUND_SIZE/SPACE_BETWEEN_TILES+1; i++) {
                DrawLine(i*SPACE_BETWEEN_TILES+1 ,0 ,i*SPACE_BETWEEN_TILES+1 ,BACKGROUND_SIZE ,LIGHTGRAY);
                DrawLine(0 ,i*SPACE_BETWEEN_TILES ,BACKGROUND_SIZE ,i*SPACE_BETWEEN_TILES ,LIGHTGRAY);
            }                     
    EndTextureMode();
}

/// @brief Zoom to mouse position
void zoomCam(Camera2D *cam) {
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
    }

    //Alternative cam (static/without mouse target)
    //cam->offset = (Vector2){(float)GetScreenWidth()/2.0f,(float)GetScreenHeight()/2.0f};
}

void moveCam(Camera2D *cam) {
    static Vector2 previousMousePosition = {0};
    //Save previous mose position to calculate drag distance between
    //old and new mouse position
    //For performance reasons this is only saved once for initial click
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        previousMousePosition = GetScreenToWorld2D(GetMousePosition(),*cam);
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(),*cam);
        Vector2 difference = {mouseWorldPos.x - previousMousePosition.x, mouseWorldPos.y - previousMousePosition.y};
        Vector2 oldTarget = cam->target;
        //Set to map boundaries when out of bounds
        //x-achsis adjustment
        cam->target.x = cam->target.x - difference.x;
        if (isCamOutOfBounds(cam)) {
            if (oldTarget.x > cam->target.x) {
                cam->target.x = 0 + cam->offset.x / cam->zoom;
            } else {
                cam->target.x = (float)BACKGROUND_SIZE - ((float)GetScreenWidth() / cam->zoom) + (cam->offset.x / cam->zoom);
            }
        }
        //y-achsis adjustment
        cam->target.y = cam->target.y - difference.y;
        if (isCamOutOfBounds(cam)) {
            if (oldTarget.y > cam->target.y) {
                cam->target.y = 0 + cam->offset.y / cam->zoom;
            } else {
                cam->target.y = (float)BACKGROUND_SIZE - ((float)GetScreenHeight() / cam->zoom) + (cam->offset.y / cam->zoom);
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
#include "../include/raylib.h"
#include <stdio.h>

#define BACKGROUND_SIZE 2000

const int screenWidth = 1920*0.8f;
const int screenHeight = 1080*0.8f;


void zoomCam(Camera2D *cam);
void prepareBackgroundTexture(RenderTexture2D *texture);
void moveCam(Camera2D *cam);
void adjustTargetToMouse(Camera2D *cam);

int main(void)
{
    // Initialization
    SetConfigFlags(FLAG_INTERLACED_HINT);
    InitWindow(screenWidth, screenHeight, "Game of Life");

    RenderTexture2D backgroundTexture = LoadRenderTexture(BACKGROUND_SIZE,BACKGROUND_SIZE);
    prepareBackgroundTexture(&backgroundTexture);
    Camera2D cam = {0};
    cam.zoom = 3.0f;



    SetTargetFPS(30);              
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key
    
        // Update
        zoomCam(&cam);
        moveCam(&cam);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode2D(cam);
            DrawTexture(backgroundTexture.texture,0,0,WHITE);

            EndMode2D();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void prepareBackgroundTexture(RenderTexture2D *texture) {
    const int spaceBetweenLines = 5;
    BeginTextureMode(*texture);
            DrawRectangle(0,0,BACKGROUND_SIZE,BACKGROUND_SIZE,RAYWHITE);
            for(int i = 0; i < BACKGROUND_SIZE/spaceBetweenLines; i++) {
                DrawLine(i*spaceBetweenLines,0,i*spaceBetweenLines,BACKGROUND_SIZE,LIGHTGRAY);
                DrawLine(0,i*spaceBetweenLines,BACKGROUND_SIZE,i*spaceBetweenLines,LIGHTGRAY);
            }
    EndTextureMode();
}

void zoomCam(Camera2D *cam) {
    const float scrollSpeed = 0.3f;
    float mouseWheelMovement = GetMouseWheelMove();

    if (mouseWheelMovement != 0) {
        adjustTargetToMouse(cam);
    }

    if(mouseWheelMovement > 0) {
        cam->zoom += scrollSpeed;
    } else if(mouseWheelMovement < 0) {
        cam->zoom -= scrollSpeed;
    }

    if(cam->zoom < 1.2f) {
        cam->zoom = 1.2f;
    } else if (cam->zoom > 6.0f) {
        cam->zoom = 6.0f;
    }
    //Alternative cam (static/without mouse target)
    //cam->offset = (Vector2){(float)GetScreenWidth()/2.0f,(float)GetScreenHeight()/2.0f};
}

void moveCam(Camera2D *cam) {
    static Vector2 previousMousePosition = {0};

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        previousMousePosition = GetScreenToWorld2D(GetMousePosition(),*cam);
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(),*cam);
        Vector2 difference = {mouseWorldPos.x - previousMousePosition.x, mouseWorldPos.y - previousMousePosition.y};
        cam->target = (Vector2){cam->target.x - difference.x, cam->target.y - difference.y};
    }
}

void adjustTargetToMouse(Camera2D *cam) {
    Vector2 mousePos = GetMousePosition();
    Vector2 mousePosWorld = GetScreenToWorld2D(mousePos,*cam);
    Vector2 offset = {GetScreenWidth() - (GetScreenWidth() - mousePos.x), GetScreenHeight() - (GetScreenHeight() - mousePos.y)};

    cam->offset = offset;
    cam->target.x = mousePosWorld.x;
    cam->target.y = mousePosWorld.y;
}
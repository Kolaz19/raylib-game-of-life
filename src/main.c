#include "../include/raylib.h"

#define BACKGROUND_SIZE 3000

void zoomCam(Camera2D *cam);
void prepareBackgroundTexture(RenderTexture2D *texture);

int main(void)
{
    // Initialization
    const int screenWidth = 1920*0.7f;
    const int screenHeight = 1080*0.7f;
    InitWindow(screenWidth, screenHeight, "Game of Life");

    RenderTexture2D backgroundTexture = LoadRenderTexture(BACKGROUND_SIZE,BACKGROUND_SIZE);
    prepareBackgroundTexture(&backgroundTexture);
    Camera2D cam = {0};
    cam.zoom = 1.0f;



    SetTargetFPS(10);              
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key
    
        // Update
        zoomCam(&cam);

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
            DrawRectangle(0,0,BACKGROUND_SIZE,BACKGROUND_SIZE,YELLOW);
            for(int i = 0; i < BACKGROUND_SIZE/spaceBetweenLines; i++) {
                DrawLine(i*spaceBetweenLines,0,i*spaceBetweenLines,BACKGROUND_SIZE,BLACK);
                DrawLine(0,i*spaceBetweenLines,BACKGROUND_SIZE,i*spaceBetweenLines,BLACK);
            }
    EndTextureMode();
}

void zoomCam(Camera2D *cam) {
    float mouseWheelMovement = GetMouseWheelMove();   
    if(mouseWheelMovement > 0) {
        cam->zoom += 0.1f;
    } else if(mouseWheelMovement < 0) {
        cam->zoom -= 0.1f;
    }
}
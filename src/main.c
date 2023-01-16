#include "../include/raylib.h"



void zoomCam(Camera2D *cam);

int main(void)
{
    // Initialization
    const int screenWidth = 1920*0.7f;
    const int screenHeight = 1080*0.7f;
    Rectangle background = {0,0,3000,3000};
    Camera2D cam = {0};
    cam.zoom = 1.0f;

    InitWindow(screenWidth, screenHeight, "Game of Life");

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
            DrawRectangle(background.x, background.y, background.width, background.height, YELLOW);
            for(int i = 0; i < 1500; i++) {
                DrawLine(i*2,0,i*2,1500,BLACK);
            }
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

void zoomCam(Camera2D *cam) {
    float mouseWheelMovement = GetMouseWheelMove();   
    if(mouseWheelMovement > 0) {
        cam->zoom += 0.02f;
    } else if(mouseWheelMovement < 0) {
        cam->zoom -= 0.02f;
    }
}
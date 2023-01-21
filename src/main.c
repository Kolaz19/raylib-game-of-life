#include "../include/raylib.h"

#define BACKGROUND_SIZE 5000

const int screenWidth = 1920*0.8f;
const int screenHeight = 1080*0.8f;


void zoomCam(Camera2D *cam);
void prepareBackgroundTexture(RenderTexture2D *texture);
void moveCam(Camera2D *cam);

int main(void)
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "Game of Life");

    RenderTexture2D backgroundTexture = LoadRenderTexture(BACKGROUND_SIZE,BACKGROUND_SIZE);
    prepareBackgroundTexture(&backgroundTexture);
    Camera2D cam = {0};
    cam.zoom = 1.0f;     //1.6f;



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
    const int spaceBetweenLines = 10;
    BeginTextureMode(*texture);
            DrawRectangle(0,0,BACKGROUND_SIZE,BACKGROUND_SIZE,YELLOW);
            for(int i = 0; i < BACKGROUND_SIZE/spaceBetweenLines; i++) {
                DrawLine(i*spaceBetweenLines,0,i*spaceBetweenLines,BACKGROUND_SIZE,BLACK);
                DrawLine(0,i*spaceBetweenLines,BACKGROUND_SIZE,i*spaceBetweenLines,BLACK);
            }
    EndTextureMode();
}

void zoomCam(Camera2D *cam) {
    const float scrollSpeed = 0.2f;
    float mouseWheelMovement = GetMouseWheelMove();   
    if(mouseWheelMovement > 0) {
        cam->zoom += scrollSpeed;
    } else if(mouseWheelMovement < 0) {
        cam->zoom -= scrollSpeed;
    }
    cam->offset = (Vector2){(float)GetScreenWidth()/2.0f,(float)GetScreenHeight()/2.0f};
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


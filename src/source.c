#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define SQUARE_SIZE 32
#define ROWS 16
#define COLS 16
#define MAX_BOMBS 20

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
// 16 x 16
static const int screenWidth = 512;
static const int screenHeight = 512;

int board[ROWS][COLS]; //game board
int bombs[ROWS][COLS]; //bomb locations
int counts[ROWS][COLS]; //num surrounding mines

static int framesCounter = 0;
static bool gameOver = false;
static bool pause = false;
static bool win = false;

static Vector2 offset = { 0 };

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)
static bool IsWinner(); // Check win state

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "breakout");

    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

bool IsWinner()
{
  return false;
}

void InitGame(void)
{
    srand(time(NULL));
    framesCounter = 0;
    gameOver = false;
    pause = false;
    win = false;
}

void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsWinner()) {
            gameOver = true;
            win = true;
        } 
        if (IsKeyPressed('P')) {
            printf("Pause toggle\n");
            pause = !pause;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER)) InitGame();
    }
}


void DrawGame(void)
{
        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (!gameOver)
        {

            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, DARKGREEN);
        }
        else 
        {

            if (win)
            {
                DrawText("WINNER! PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("WINNER! PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, DARKGREEN);
            }
            else
            {
                DrawText("GAME OVER! PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("GAME OVER! PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, DARKGREEN);
            }
        }

    EndDrawing();
}

void UnloadGame(void)
{
    //Unload all dynamic loaded data
}

void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

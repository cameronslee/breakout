#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define PLAYER_HEIGHT 16
#define PLAYER_WIDTH 48
#define ROWS_BRICKS 4
#define COLS_BRICKS 7

//------------------------------------------------------------------------------------
// Types n Structs 
//------------------------------------------------------------------------------------
typedef struct Player {
    Vector2 position;
    Vector2 size;
    float speed;
} Player;

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    int radius;
} Ball;

typedef struct Brick {
    Vector2 position;
    bool active;
} Brick;  

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
// 16 x 16
static const int screenWidth = 512;
static const int screenHeight = 512;

static unsigned int score = 0;

static int framesCounter = 0;
static bool gameOver = false;
static bool pause = false;
static bool win = false;

static Brick bricks[ROWS_BRICKS][COLS_BRICKS] = { 0 };
static Vector2 brickSize = { 0 };
Player player;
Ball ball;
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
    brickSize = (Vector2){ 64, 32};

    srand(time(NULL));
    framesCounter = 0;
    gameOver = false;
    pause = false;
    win = false;
    score = 0;

    // Init Player
    player.size.x = PLAYER_WIDTH;
    player.size.y = PLAYER_HEIGHT;
    player.position.x = screenWidth / 2;
    player.position.y = screenHeight - player.size.x;
    player.speed = 10.0;

    // Init Ball
    ball.position.x = screenWidth / 2;
    ball.position.y = screenHeight / 2;
    ball.speed.x = 5.0;
    ball.speed.y = 5.0;
    ball.radius = 8;

    // Init Bricks
    for (int i = 0; i < ROWS_BRICKS; i++) {
      for (int j = 0; j < COLS_BRICKS; j++) {
        bricks[i][j].position = (Vector2){ j*brickSize.x + 32 , i*brickSize.y + 8};
        bricks[i][j].active = true;
      }
    }

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
          pause = !pause;
        }

        if (IsKeyDown('A')) player.position.x -= player.speed;
        if ((player.position.x - player.size.x/2) <= 0) player.position.x = player.size.x/2;

        if (IsKeyDown('D')) player.position.x += player.speed;
        if ((player.position.x + player.size.x/2) >= screenWidth) player.position.x = screenWidth - player.size.x/2;
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

        // Draw Player
        DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, DARKGREEN);

        //Draw Score
        char score_buff[4];
        sprintf(score_buff, "%d", score);
        DrawText(score_buff, 10, screenHeight - 20, 20, DARKBLUE);

        // Draw Ball
        DrawCircle(ball.position.x, ball.position.y, ball.radius, RED);
        
        // Draw Bricks
        for (int i = 0; i < ROWS_BRICKS; i++) {
          for (int j = 0; j < COLS_BRICKS; j++) {
            if (bricks[i][j].active) DrawRectangle(bricks[i][j].position.x, bricks[i][j].position.y, brickSize.x-8, brickSize.y-8, GRAY);
          }
        }

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

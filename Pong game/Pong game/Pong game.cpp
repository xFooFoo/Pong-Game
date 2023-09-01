/*Maybe reset paddles to middle? Better green colours. Adjust screen res*/

#include <iostream>
#include <raylib.h>
using namespace std;

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw(){
        //x & y are the centre of the circle
        DrawCircle(x, y, radius, RED);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }

        if (x + radius >= GetScreenWidth()) {
            player_score++;
            resetBall();
        }
        if (x - radius <= 0) {
            cpu_score++;
            resetBall();
        }
    }

    void resetBall() {
        x = GetScreenHeight() / 2;
        y = GetScreenWidth() / 2;
        int speedChoices[2] = { -1, 1 };
        speed_x *= speedChoices[GetRandomValue(0, 1)];
        speed_y *= speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle {
public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        //x & y are the top left corner of the paddle
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.6, 0, RAYWHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            if (y > 0) {
                y -= speed;
            }
        }
        else if (IsKeyDown(KEY_DOWN)) {
            if (y + height < GetScreenHeight()) {
                y += speed;
            }
        }
    }
};

class CpuPaddle : public Paddle {
public:
    //Override to draw as Dark Purple
    void Draw() {
        //x & y are the top left corner of the paddle
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.6, 0, RAYWHITE);
    }

    void Update(int ball_y) {
        if (y + height / 2 > ball_y  && y > 0) {
            y -= speed;
        }
        else if (y + height / 2 <  ball_y && y + height < GetScreenHeight()) {
            y += speed;
        }
    }
};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1000;
    const int screenHeight = 800;
    const int rectangleWidth = 20;
    const int rectangleHeight = 100;
    const int OFFSET = 10;

    Color CustomDarkGreen = { 41, 121, 89, 255 };
    Color CustomLightGreen = { 84, 148, 122, 255 };
    Color LighterGreen = { 118, 169, 149, 255 };


    Ball ball;
    ball.radius = 20;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    Paddle player;
    player.width = rectangleWidth;
    player.height = rectangleHeight;
    player.x = OFFSET;
    player.y = screenHeight / 2 - rectangleHeight / 2;
    player.speed = 6;

    CpuPaddle cpu;
    cpu.width = rectangleWidth;
    cpu.height = rectangleHeight;
    cpu.x = screenWidth - rectangleWidth - OFFSET;
    cpu.y = screenHeight / 2 - rectangleHeight / 2;
    cpu.speed = 6;

    InitWindow(screenWidth, screenHeight, "Foo Pong");
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        // UPDATING
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // CHECKING FOR COLLISIONS
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height })) {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height })) {
            ball.speed_x *= -1;
        }
        // DRAWING
        //----------------------------------------------------------------------------------
        //PLAYING FIELD DESIGN
        ClearBackground(CustomDarkGreen);
        DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, CustomLightGreen);
        DrawCircle(screenWidth / 2, screenHeight / 2, screenHeight / 8, LighterGreen);
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, RAYWHITE);
        // BALL & PADDLES
        ball.Draw();
        player.Draw();
        cpu.Draw();
        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        DrawText(TextFormat("%i", cpu_score), 3 * screenWidth / 4, 20, 80, RAYWHITE);
        DrawText(TextFormat("%i", player_score), screenWidth / 4 - 20, 20, 80, RAYWHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <random>
#include <vector>
#include <set>
#include <algorithm>

bool inRange(float low, float high, float x)
{
    return (low < x && x < high);
}

std::mt19937 generator;

int randint(int min, int max){
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

float randFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

class dot{
    public:
        Vector2 vel, pos, start;

        dot(int screenWidth, int screenHeight){
            switch(randint(0, 4)){
                case 0: // Starts on bottom
                    pos.x = randFloat(0+50, screenWidth-50);
                    pos.y = 0;
                    vel.x = randFloat(-1.5, 1.5); //Random -1 to 1 velocity on x
                    vel.y = randFloat(1, 1.5); //Random -1 to 1 velocity on y
                    if (vel.x == 0) vel.x = 1;
                    if (vel.y == 0) vel.y = 1;
                    break;
                case 1: // Starts on top
                    pos.x = randFloat(0+50, screenWidth-50);
                    pos.y = screenHeight-1;
                    vel.x = randFloat(-1.5, 1.5); //Random -1 to 1 velocity on x
                    vel.y = -randFloat(1, 1.5); //Random -1 to 1 velocity on y
                    if (vel.x == 0) vel.x = -1;
                    if (vel.y == 0) vel.y = -1;
                    break;
                case 2: // Starts on left
                    pos.x = 0;
                    pos.y = randFloat(0+50, screenHeight-50);
                    vel.x = randFloat(1, 1.5); //Random -1 to 1 velocity on x
                    vel.y = randFloat(-1.5, 1.5); //Random -1 to 1 velocity on y
                    if (vel.x == 0) vel.x = 1;
                    if (vel.y == 0) vel.y = 1;
                    break;
                case 3: // Starts on right
                    pos.x = screenWidth-1;
                    pos.y = randFloat(0+50, screenHeight-50);
                    vel.x = -randFloat(1, 1.5); //Random -1 to 1 velocity on x
                    vel.y = randFloat(-1.5, 1.5); //Random -1 to 1 velocity on y
                    if (vel.x == 0) vel.x = -1;
                    if (vel.y == 0) vel.y = -1;
                    break;
            };

            start.x = pos.x;
            start.y = pos.y;
        };

        dot(float vx, float vy, int sx, int sy){
            vel.x = vx;
            vel.y = vy;
            pos.x = sx;
            pos.y = sy;
            start.x = pos.x;
            start.y = pos.y;
        };

        void move(int screenWidth, int screenHeight){
            pos.x = pos.x + vel.x;
            pos.y = pos.y + vel.y;

            if (!inRange(0, screenWidth, pos.x) || !inRange(0, screenHeight, pos.y)) {
                pos.x = start.x;
                pos.y = start.y;
            }
        }
};

float distance(int x1, int y1, int x2, int y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2) + 
                pow(y2 - y1, 2) * 1.0);
}

struct line {
    Vector2 v1, v2;
};

bool vec_equal(Vector2 &a, Vector2 &b) {
    return a.x==b.x&&a.y==b.y;
}
bool line_equal(line &a, line &b) {
    return vec_equal(a.v1, b.v1)&&vec_equal(a.v2, b.v2);
}

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Please wait, Initializing.", 10, 10, 20, LIME);
    EndDrawing();

    int currentBalls = 250;
    int targetBalls = 250;
    double timeToDraw = 0;
    double tempTime = GetTime();
    int lastscroll = 0;
    int scrollMul = 1;
    int targetfps = 0;
    std::vector<dot> balls;


    for (int i = 0; i < 250; i++) balls.push_back(dot(screenWidth, screenHeight));

    //--------------------------------------------------------------------------------------
    
    SetTargetFPS(60);

    while (lastscroll == 0){
        BeginDrawing();

        lastscroll = GetMouseWheelMove();
        ClearBackground(RAYWHITE);
        DrawText("Please scroll up to calibrate scrolling", 10, 10, 20, LIME);

        EndDrawing();
    }

    scrollMul = 5/lastscroll;
    
    SetTargetFPS(1);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Scrolling calibrated, starting.", 10, 10, 20, LIME);
    EndDrawing();

    SetTargetFPS(60);
    targetfps = 60;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key and exit if pressed
    {
        if (IsWindowFocused()){
            SetTargetFPS(targetfps);
            if(GetKeyPressed() == KEY_M){
                if (targetfps == 0){
                    targetfps = 60;
                    SetTargetFPS(60);
                }
                else {
                    targetfps = 0;
                    SetTargetFPS(0);
                }
            }
            

            tempTime = GetTime();
            targetBalls = std::max(1.0f, targetBalls + GetMouseWheelMove() * scrollMul);

            while (currentBalls != targetBalls){
                if (currentBalls > targetBalls){
                    balls.pop_back();
                    currentBalls--;
                }
                else {
                    balls.push_back(dot(screenWidth, screenHeight));
                    currentBalls++;
                }
            }

            // Update
            for (auto& i : balls) { 
                i.move(screenWidth, screenHeight);
            } 

            std::vector<line> lines;

            for (auto& b1 : balls) { 
                for (auto& b2 : balls) { 
                    if (distance(b1.pos.x, b1.pos.y, b2.pos.x, b2.pos.y) < 50) {
                        line e = {b1.pos, b2.pos};
                        if (std::find_if(lines.begin(), lines.end(), [&](line& s) { return line_equal(s, e); }) == lines.end()){
                            lines.push_back(
                                {
                                    b1.pos, b2.pos
                                }
                            );
                        }
                    }
                }
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();
            ClearBackground(RAYWHITE);

            for (auto& i : balls) { 
                DrawCircle(i.pos.x, i.pos.y, 2, BLACK);
            } 

            for (auto& i : lines) { 
                DrawLineEx(i.v1, i.v2, 3, {0, 0, 0, (unsigned char)(255-distance(i.v1.x, i.v1.y, i.v2.x, i.v2.y)/50*255)});
            } 

            DrawText(TextFormat("%2i FPS | %2i\n%2i Balls", GetFPS(), (int)(1/timeToDraw), targetBalls), 10, 10, 20, LIME);
            DrawText(targetfps == 0 ? "Fps uncapped\nScroll to change ball count, press ESC to quit" : "Fps capped\nScroll to change ball count, press ESC to quit", 10, screenHeight-55, 20, LIME);
            timeToDraw = GetTime() - tempTime;
            EndDrawing();
            //----------------------------------------------------------------------------------
        }
        else {
            SetTargetFPS(1);
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(TextFormat("%2i FPS\nPaused", GetFPS()), 10, 10, 20, LIME);
            EndDrawing();
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
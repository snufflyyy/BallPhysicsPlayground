#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAXBALLS 1000

typedef struct {
    float ballRadius;
    Vector2 ballPosition;
    Vector2 ballVelocity;
    Color ballColor;
} ball;

const int windowWidth = 800;
const int windowHeight = 600;

// world variables
const float gravity = 0.5f;
const float airResistance = 1;
const float friction = 2.5f;

// 1000 is the max amount of balls possible
ball balls[MAXBALLS];
int numberOfBalls;

void spawnBall() {
    if (IsKeyPressed(KEY_SPACE) && numberOfBalls != MAXBALLS) {
        srand(time(NULL));
        balls[numberOfBalls].ballRadius = rand() % 25 + 1;
        balls[numberOfBalls].ballPosition.x = (float) windowWidth / 2;
        balls[numberOfBalls].ballPosition.y = balls[numberOfBalls].ballRadius;
        balls[numberOfBalls].ballVelocity.x = 0;
        balls[numberOfBalls].ballColor = BLUE;
        numberOfBalls++;
    }
}

void updateBalls() {
    for (int i = 0; i < numberOfBalls; i++) {
        balls[i].ballVelocity.y += gravity;

        balls[i].ballPosition.x += balls[i].ballVelocity.x * airResistance;
        balls[i].ballPosition.y += balls[i].ballVelocity.y * airResistance;

        // floor
        if (balls[i].ballPosition.y > (float) windowHeight - balls[i].ballRadius) {
            balls[i].ballPosition.y = (float) windowHeight - balls[i].ballRadius;
            balls[i].ballVelocity.y = -balls[i].ballVelocity.y + friction;
        }

        if (balls[i].ballPosition.x < 0 + balls[i].ballRadius) {
            balls[i].ballPosition.x = balls[i].ballRadius;
            balls[i].ballVelocity.x = -balls[i].ballVelocity.x - friction;
        }

        if (balls[i].ballPosition.x > (float) windowWidth - balls[i].ballRadius) {
            balls[i].ballPosition.x = (float) windowWidth - balls[i].ballRadius;
            balls[i].ballVelocity.x = -balls[i].ballVelocity.x + friction;
        }
    }

    if (numberOfBalls > 1) {
        for (int i = 0; i < numberOfBalls - 1; i++) {
            for (int b = 1; b < numberOfBalls; b++) {
                float dX = 0;
                float dY = 0;

                bool isBGreaterX = false;
                bool isBGreaterY = false;

                if (balls[b].ballPosition.x > balls[i].ballPosition.x) {
                    dX = balls[b].ballPosition.x - balls[i].ballPosition.x;
                    isBGreaterX = true;
                } else {
                    dX = balls[i].ballPosition.x - balls[b].ballPosition.x;
                    isBGreaterX = false;
                }

                if (balls[b].ballPosition.y > balls[i].ballPosition.y) {
                    dY = balls[b].ballPosition.y - balls[i].ballPosition.y;
                    isBGreaterY = true;
                } else {
                    dY = balls[i].ballPosition.y - balls[b].ballPosition.y;
                    isBGreaterY = false;
                }

                if (sqrtf(dX * dX + dY * dY) < balls[b].ballRadius + balls[i].ballRadius) {
                    if (isBGreaterX) {
                        balls[b].ballPosition.x += (balls[b].ballRadius + balls[i].ballRadius - sqrtf(dX * dX)) / 2;
                        balls[b].ballVelocity.x += 1;
                        balls[i].ballPosition.x -= (balls[b].ballRadius + balls[i].ballRadius - sqrtf(dX * dX)) / 2;
                        balls[i].ballVelocity.x -= 1;
                    } else {
                        balls[i].ballPosition.x += (balls[b].ballRadius + balls[i].ballRadius - sqrtf(dX * dX)) / 2;
                        balls[b].ballVelocity.x -= 1;
                        balls[b].ballPosition.x -= (balls[b].ballRadius + balls[i].ballRadius - sqrtf(dX * dX)) / 2;
                        balls[i].ballVelocity.x += 1;
                    }

                    if (isBGreaterY) {
                        balls[b].ballPosition.y += (balls[b].ballRadius + balls[i].ballRadius - sqrtf(dY * dY)) / 2;
                        balls[i].ballPosition.y -= (balls[b].ballRadius + balls[i].ballRadius - sqrtf(dY * dY)) / 2;
                    } else {
                        balls[i].ballPosition.y += (balls[b].ballRadius + balls[i].ballRadius - sqrtf(dY * dY)) / 2;
                        balls[b].ballPosition.y -= (balls[b].ballRadius + balls[i].ballRadius - sqrtf(dY * dY)) / 2;
                    }
                }
            }
        }
    }
}

int main() {
    InitWindow(windowWidth, windowHeight, "Bouncing Ball");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        spawnBall();
        updateBalls();

        BeginDrawing();
            ClearBackground(BLACK);

            DrawFPS(25, 25);

            for (int i = 0; i < numberOfBalls; i++) {
                DrawCircle(balls[i].ballPosition.x, balls[i].ballPosition.y, balls[i].ballRadius, balls[i].ballColor);
            }
        EndDrawing();
    }
}
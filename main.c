#include <raylib.h>

const int windowWidth = 800;
const int windowHeight = 600;

// world variables
const float gravity = 0.5f;
const float airResistance = 1;
const float friction = 2.5f;

// bouncing ball variables
const float ballRadius = 25;
Vector2 ballPosition = {(float) windowWidth / 2, ballRadius};
Vector2 ballVelocity = {10, 0};

void updateBall() {
    ballVelocity.y += gravity;

    ballPosition.x += ballVelocity.x * airResistance;
    ballPosition.y += ballVelocity.y * airResistance;

    // floor
    if (ballPosition.y > (float) windowHeight - ballRadius) {
        ballPosition.y = (float) windowHeight - ballRadius;
        ballVelocity.y = -ballVelocity.y + friction;
    }

    if (ballPosition.x < 0 + ballRadius) {
        ballPosition.x = ballRadius;
        ballVelocity.x = -ballVelocity.x - friction;
    }

    if (ballPosition.x > (float) windowWidth - ballRadius) {
        ballPosition.x = (float) windowWidth - ballRadius;
        ballVelocity.x = -ballVelocity.x + friction;
    }
}

int main() {
    InitWindow(windowWidth, windowHeight, "Bouncing Ball");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        updateBall();

        BeginDrawing();
        ClearBackground(BLACK);
            DrawCircle(ballPosition.x, ballPosition.y, ballRadius, BLUE);
        EndDrawing();
    }
}
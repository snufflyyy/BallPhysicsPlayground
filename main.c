#include <raylib.h>
#include <stdio.h>
#include <math.h>

#define MAXBALLAMOUNT 2000

typedef struct {
    Vector2 currentPosition;
    Vector2 lastPosition;
    Vector2 acceleration;
    float radius;
    Color color;
} ball;

const int windowWidth = 1280;
const int windowHeight = 720;

const int subSteps = 2;

const Vector2 gravity = {0, 2500};

// 1000 is the max amount of balls allowed
ball balls[MAXBALLAMOUNT];
int ballAmount = 0;

void createBall() {
    if (ballAmount != MAXBALLAMOUNT) {
        balls[ballAmount].radius = 25;
        balls[ballAmount].currentPosition.x = windowWidth / 2;
        balls[ballAmount].currentPosition.y = balls[ballAmount].radius * 3;
        balls[ballAmount].lastPosition.x = balls[ballAmount].currentPosition.x;
        balls[ballAmount].lastPosition.y = balls[ballAmount].currentPosition.y; 
        balls[ballAmount].color = BLUE;
        ballAmount++;
    }
}

void applyGravity() {
    for (int i = 0; i < ballAmount; i++) {
        balls[i].acceleration.x = gravity.x;
        balls[i].acceleration.y = gravity.y;
    }
}

void updateBalls(float deltaTime) {
    for (int i = 0; i < ballAmount; i++) {
        Vector2 velocity = {balls[i].currentPosition.x - balls[i].lastPosition.x, balls[i].currentPosition.y - balls[i].lastPosition.y};

        balls[i].lastPosition.x = balls[i].currentPosition.x;
        balls[i].lastPosition.y = balls[i].currentPosition.y;

        balls[i].currentPosition.x = balls[i].currentPosition.x + velocity.x + balls[i].acceleration.x * deltaTime * deltaTime;
        balls[i].currentPosition.y = balls[i].currentPosition.y + velocity.y + balls[i].acceleration.y * deltaTime * deltaTime;

        balls[i].acceleration.x = 0;
        balls[i].acceleration.y = 0;
    }
}

void applyConstraints() {
    for (int i = 0; i < ballAmount; i++) {
        if (balls[i].currentPosition.y > windowHeight - balls[i].radius) {
            balls[i].currentPosition.y = windowHeight - balls[i].radius;
        } else if (balls[i].currentPosition.y < balls[i].radius) {
            balls[i].currentPosition.y = balls[i].radius;
        }

        if (balls[i].currentPosition.x < balls[i].radius) {
            balls[i].currentPosition.x = balls[i].radius;
        } else if (balls[i].currentPosition.x > windowWidth - balls[i].radius) {
            balls[i].currentPosition.x = windowWidth - balls[i].radius;
        }
    }
}

void getCollisions() {
    for (int i = 0; i < ballAmount; i++) {
        for (int b = i + 1; b < ballAmount; b++) {
            Vector2 collisionAxis = {balls[i].currentPosition.x - balls[b].currentPosition.x, balls[i].currentPosition.y - balls[b].currentPosition.y};
            float distance = sqrtf(collisionAxis.x * collisionAxis.x + collisionAxis.y * collisionAxis.y);
            float minimumDistance = balls[i].radius + balls[b].radius;

            if (distance < minimumDistance) {
                Vector2 n = {collisionAxis.x / distance, collisionAxis.y / distance};
                float delta = minimumDistance - distance;
                balls[i].currentPosition.x += 0.5f * delta * n.x;
                balls[i].currentPosition.y += 0.5f * delta * n.y;
                balls[b].currentPosition.x -= 0.5f * delta * n.x;
                balls[b].currentPosition.y -= 0.5f * delta * n.y;
            }

        }    
    }
}

int main() {
    SetTargetFPS(240);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, "Balls");

    while (!WindowShouldClose()) {
        const float subStepsDeltaTime = GetFrameTime() / (float) subSteps;

        if (IsKeyDown(KEY_SPACE)) { createBall(); }

        for (int i = subSteps; i >= 0; i--) {
            applyGravity();
            updateBalls(subStepsDeltaTime);
            applyConstraints();
            getCollisions();

            for (int i = 0; i < ballAmount; i++) {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(GetMousePosition(), balls[i].currentPosition, balls[i].radius)) {
                balls[i].currentPosition.x = GetMouseX();
                balls[i].currentPosition.y = GetMouseY();
                }
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            
            for (int i = 0; i < ballAmount; i++) {
                DrawCircle(balls[i].currentPosition.x, balls[i].currentPosition.y, balls[i].radius, balls[i].color);
            }

            DrawFPS(0, 0);
            DrawText(TextFormat("%d", ballAmount), 0, 10, 25, WHITE);
        EndDrawing();

    }
    
    return 0;
}
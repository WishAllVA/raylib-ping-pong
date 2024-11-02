#include<stdio.h>
#include<raylib.h>

int player_score = 0;
int cpu_score = 0;
bool is_game_ended = false;

enum Player {
    PLAYER,
    CPU
};

typedef struct {
    int ballX;
    int ballY;
    int ballSpeedX;
    int ballSpeedY;
    int ballRadius;
    Color ballColor;
} Ball;

typedef struct {
    int paddleX;
    int paddleY;
    int paddleWidth;
    int paddleHeight;
    int paddleSpeed;
    Color paddleColor;
} Paddle;

void drawBall (Ball *ball) {
    DrawCircle(ball->ballX, ball->ballY, ball->ballRadius, ball->ballColor);
}

void endGame (Ball *ball, enum Player winner) {
    ball->ballX = GetScreenWidth() / 2;
    ball->ballY = GetScreenHeight() / 2;
    ball->ballSpeedX = 0;
    ball->ballSpeedY = 0;
    if (winner == 0) {
        DrawText(TextFormat("Player won"), GetScreenWidth() / 2 - 130, GetScreenHeight() / 2, 40, WHITE);
    } else {
        DrawText(TextFormat("CPU won"), GetScreenWidth() / 2 - 80, GetScreenHeight() / 2, 40, WHITE);
    }
}

void updateBall (Ball *ball) {
    ball->ballX += ball->ballSpeedX;
    ball->ballY += ball->ballSpeedY;

    if (ball->ballX <= ball->ballRadius) cpu_score++;
    if (ball->ballX >= GetScreenWidth() - ball->ballRadius) player_score++;

    if (cpu_score >= 3) {
        enum Player winner = CPU;
        endGame(ball, winner);
        is_game_ended = true;
        return;
    }
    if (player_score >= 3) {
        enum Player winner = PLAYER;
        endGame(ball, winner);
        is_game_ended = true;
        return;
    }

    if (ball->ballY + ball->ballRadius >= GetScreenHeight()) {
        ball->ballY = GetScreenHeight() - ball->ballRadius;
        ball->ballSpeedY = -ball->ballSpeedY;
    }
    if (ball->ballX + ball->ballRadius >= GetScreenWidth()) {
        ball->ballX = GetScreenWidth() - ball->ballRadius;
        ball->ballSpeedX = -ball->ballSpeedX;
    }
    if (ball->ballY - ball->ballRadius <= 0) {
        ball->ballY = ball->ballRadius;
        ball->ballSpeedY = -ball->ballSpeedY;
    }
    if (ball->ballX - ball->ballRadius <= 0) {
        ball->ballX = ball->ballRadius;
        ball->ballSpeedX = -ball->ballSpeedX;
    }
}

void drawPaddle (Paddle *paddle) {
    DrawRectangle(paddle->paddleX, paddle->paddleY, paddle->paddleWidth, paddle->paddleHeight, paddle->paddleColor);
}

void updatePaddle (Paddle *paddle, Ball *ball) {
    if (IsKeyDown(KEY_UP)) {
        paddle->paddleY -= paddle->paddleSpeed;
    } else if (IsKeyDown(KEY_DOWN)) {
        paddle->paddleY += paddle->paddleSpeed;
    }
    if (paddle->paddleY <= 0) {
        paddle->paddleY = 0;
    } else if (paddle->paddleY >= GetScreenHeight() - paddle->paddleHeight) {
        paddle->paddleY = GetScreenHeight() - paddle->paddleHeight;
    }
}

void updateCpuPaddle (Paddle *paddle, Ball *ball) {
    if (paddle->paddleY > ball->ballY && paddle->paddleSpeed > 0) {
        paddle->paddleSpeed = -paddle->paddleSpeed;
    } else if (paddle->paddleY < ball->ballY && paddle->paddleSpeed < 0) {
        paddle->paddleSpeed = -paddle->paddleSpeed;
    }
    paddle->paddleY += paddle->paddleSpeed;
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 400;
    InitWindow(screenWidth, screenHeight, "Ping Pong!");
    SetTargetFPS(60);
    Ball ball;
    ball.ballX = GetScreenWidth() / 2;
    ball.ballY = GetScreenHeight() / 2;
    ball.ballRadius = 10;
    ball.ballSpeedX = 9;
    ball.ballSpeedY = 9;
    ball.ballColor = WHITE;

    Paddle paddle1, paddle2;
    paddle1.paddleColor = WHITE;
    paddle2.paddleColor = WHITE;
    paddle1.paddleX = 30;
    paddle2.paddleX = GetScreenWidth() - 45;
    paddle1.paddleY = (GetScreenWidth() / 2) - 40;
    paddle2.paddleY = (GetScreenHeight() / 2) - 40;
    paddle1.paddleWidth = 15;
    paddle2.paddleWidth = 15;
    paddle1.paddleHeight = 80;
    paddle2.paddleHeight = 80;
    paddle1.paddleSpeed = 5;
    paddle2.paddleSpeed = 5;

    while (!WindowShouldClose()) {
        BeginDrawing();
        if (is_game_ended && IsKeyDown(KEY_ENTER)) {
            is_game_ended = false;
            ball.ballSpeedX = 9;
            ball.ballSpeedY = 9;
            player_score = 0;
            cpu_score = 0;
        }
        updateBall(&ball);
        if (!is_game_ended) {
            updatePaddle(&paddle1, &ball);
            updateCpuPaddle(&paddle2, &ball);
            // Checking for collisions
            if (CheckCollisionCircleRec((Vector2){ball.ballX, ball.ballY}, ball.ballRadius, (Rectangle){ paddle1.paddleX, paddle1.paddleY, paddle1.paddleWidth, paddle1.paddleHeight })) {
                ball.ballSpeedX = -ball.ballSpeedX;
            }

            if (CheckCollisionCircleRec((Vector2){ball.ballX, ball.ballY}, ball.ballRadius, (Rectangle){ paddle2.paddleX, paddle2.paddleY, paddle1.paddleWidth, paddle2.paddleHeight })) {
                ball.ballSpeedX = -ball.ballSpeedX;
            }   
        } else {
            DrawText("New game? Press ENTER to start", 130, 60, 30, GREEN);
        }

        ClearBackground(BLACK);

        drawPaddle(&paddle1);
        drawPaddle(&paddle2);

        drawBall(&ball);

        DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), WHITE);
        DrawText(TextFormat("%i", player_score), 30, 10, 60, WHITE);
        DrawText(TextFormat("%i", cpu_score), GetScreenWidth() / 2 + 30, 10, 60, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
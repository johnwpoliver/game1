#include "GameOverScene.h"
#include "IntroScene.h"
#include <cstdlib>

void GameOverScene::onEnter() {
    SDL_Log("GameOverScene: Enter (%s)", playerWon ? "WIN" : "LOSE");
    timer = 0.0f;

    // Initialize blocks with random positions and velocities
    for (int i = 0; i < numBlocks; i++) {
        // Random size
        blocks[i].setSize(20.0f + (i % 4) * 10.0f);
        blocks[i].setBreathOffset(i * 0.5f);

        if (playerWon) {
            // Win: blocks start at bottom, rise up
            float x = 50.0f + (i * 60.0f);
            blocks[i].setPosition(x, 650.0f);  // Start below screen
            velocityX[i] = ((i % 3) - 1) * 30.0f;  // Slight horizontal drift
            velocityY[i] = -150.0f - (i * 20.0f);  // Rise up at different speeds

            // Gold/green celebration colors
            if (i % 3 == 0) {
                blocks[i].setColor(255, 215, 0);   // Gold
            } else if (i % 3 == 1) {
                blocks[i].setColor(50, 205, 50);   // Lime green
            } else {
                blocks[i].setColor(255, 255, 100); // Yellow
            }
        } else {
            // Lose: blocks start at top, fall down
            float x = 50.0f + (i * 60.0f);
            blocks[i].setPosition(x, -50.0f);  // Start above screen
            velocityX[i] = ((i % 3) - 1) * 20.0f;  // Slight horizontal drift
            velocityY[i] = 50.0f + (i * 15.0f);    // Fall at different speeds

            // Dark/red sad colors
            if (i % 3 == 0) {
                blocks[i].setColor(139, 0, 0);     // Dark red
            } else if (i % 3 == 1) {
                blocks[i].setColor(80, 80, 80);    // Dark gray
            } else {
                blocks[i].setColor(128, 0, 0);     // Maroon
            }
        }
    }
}

void GameOverScene::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN ||
        event.type == SDL_EVENT_FINGER_DOWN ||
        event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        requestReplace<IntroScene>();
    }
}

void GameOverScene::update(float deltaTime) {
    timer += deltaTime;
    if (timer > 10.0f) {
        requestReplace<IntroScene>();
    }

    // Update block positions
    for (int i = 0; i < numBlocks; i++) {
        // Apply gravity effect
        if (playerWon) {
            velocityY[i] += 50.0f * deltaTime;  // Slow down rise (gravity pulling back)
        } else {
            velocityY[i] += 100.0f * deltaTime; // Accelerate fall (gravity)
        }

        // Move blocks
        blocks[i].move(velocityX[i] * deltaTime, velocityY[i] * deltaTime);

        // Update breathing animation
        blocks[i].update(deltaTime);

        // Wrap horizontally
        float x = blocks[i].getX();
        if (x < -50.0f) blocks[i].setPosition(850.0f, blocks[i].getY());
        if (x > 850.0f) blocks[i].setPosition(-50.0f, blocks[i].getY());
    }
}

void GameOverScene::render(SDL_Renderer* renderer) {
    // Background color
    if (playerWon) {
        SDL_SetRenderDrawColor(renderer, 30, 80, 30, 255);  // Dark green
    } else {
        SDL_SetRenderDrawColor(renderer, 80, 30, 30, 255);  // Dark red
    }
    SDL_RenderClear(renderer);

    // Render animated blocks
    for (int i = 0; i < numBlocks; i++) {
        blocks[i].render(renderer);
    }

    // Draw main text
    SDL_SetRenderScale(renderer, 4.0f, 4.0f);
    if (playerWon) {
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);  // Gold
        SDL_RenderDebugText(renderer, 50, 25, "YOU WIN!");
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);  // Light red
        SDL_RenderDebugText(renderer, 40, 25, "GAME OVER");
    }
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // Draw subtitle
    SDL_SetRenderScale(renderer, 2.0f, 2.0f);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    if (playerWon) {
        SDL_RenderDebugText(renderer, 130, 80, "Congratulations!");
    } else {
        SDL_RenderDebugText(renderer, 140, 80, "Better luck next time");
    }
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // Draw "Press any key" with blinking effect
    int blink = (int)(timer * 2) % 2;
    if (blink == 0) {
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(renderer, 130, 250, "Press any key");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }
}

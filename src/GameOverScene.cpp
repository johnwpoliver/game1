#include "GameOverScene.h"
#include "IntroScene.h"
#include "DisplayManager.h"
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
            blocks[i].setPosition(x, DisplayManager::DESIGN_HEIGHT + 50.0f);  // Start below screen
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
        const float margin = 50.0f;
        if (x < -margin) blocks[i].setPosition(DisplayManager::DESIGN_WIDTH + margin, blocks[i].getY());
        if (x > DisplayManager::DESIGN_WIDTH + margin) blocks[i].setPosition(-margin, blocks[i].getY());
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

    // Draw main text (at actual y ~80)
    SDL_SetRenderScale(renderer, 4.0f, 4.0f);
    if (playerWon) {
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);  // Gold
        SDL_RenderDebugText(renderer, 50, 20, "YOU WIN!");
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);  // Light red
        SDL_RenderDebugText(renderer, 40, 20, "GAME OVER");
    }
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // Draw scores (at actual y ~180 and ~230)
    char scoreText[64];
    const float scoreScale = 2.0f;
    SDL_SetRenderScale(renderer, scoreScale, scoreScale);

    // Current score
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    snprintf(scoreText, sizeof(scoreText), "SCORE: %d", finalScore);
    float scoreWidth = strlen(scoreText) * 8.0f;
    float scoreX = (DisplayManager::DESIGN_WIDTH / scoreScale - scoreWidth) / 2.0f;
    SDL_RenderDebugText(renderer, scoreX, 90, scoreText);  // actual y = 180

    // High score (highlighted if new)
    bool isNewHighScore = (finalScore >= highScore && finalScore > 0);
    if (isNewHighScore) {
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);  // Gold for new high score
        snprintf(scoreText, sizeof(scoreText), "NEW HIGH SCORE!");
    } else {
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        snprintf(scoreText, sizeof(scoreText), "HIGH SCORE: %d", highScore);
    }
    float highScoreWidth = strlen(scoreText) * 8.0f;
    float highScoreX = (DisplayManager::DESIGN_WIDTH / scoreScale - highScoreWidth) / 2.0f;
    SDL_RenderDebugText(renderer, highScoreX, 115, scoreText);  // actual y = 230

    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // Draw subtitle (at actual y ~310)
    const float subtitleScale = 1.5f;
    SDL_SetRenderScale(renderer, subtitleScale, subtitleScale);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    if (playerWon) {
        float subtitleWidth = 16 * 8.0f;  // "Congratulations!" is 16 chars
        float subtitleX = (DisplayManager::DESIGN_WIDTH / subtitleScale - subtitleWidth) / 2.0f;
        SDL_RenderDebugText(renderer, subtitleX, 207, "Congratulations!");
    } else {
        float subtitleWidth = 21 * 8.0f;  // "Better luck next time" is 21 chars
        float subtitleX = (DisplayManager::DESIGN_WIDTH / subtitleScale - subtitleWidth) / 2.0f;
        SDL_RenderDebugText(renderer, subtitleX, 207, "Better luck next time");
    }
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // Draw "Press any key" with blinking effect (at actual y ~500)
    int blink = (int)(timer * 2) % 2;
    if (blink == 0) {
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        float pressWidth = 13 * 8.0f;  // "Press any key" is 13 chars
        float pressX = (DisplayManager::DESIGN_WIDTH / 2.0f - pressWidth) / 2.0f;
        SDL_RenderDebugText(renderer, pressX, 250, "Press any key");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }
}

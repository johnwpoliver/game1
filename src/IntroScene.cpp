#include "IntroScene.h"
#include "LevelIntroScene.h"
#include <cmath>

void IntroScene::onEnter() {
    SDL_Log("IntroScene: Enter");
    timer = 0.0f;

    // Initialize orbiting blocks with different colors and breath offsets
    for (int i = 0; i < numBlocks; i++) {
        orbitBlocks[i].setSize(24.0f);
        orbitBlocks[i].setBreathOffset(i * 1.0f);  // Stagger breathing phases

        // Color varies per block
        Uint8 r = 100 + (i * 25) % 156;
        Uint8 g = 150 + (i * 40) % 106;
        Uint8 b = 200 + (i * 15) % 56;
        orbitBlocks[i].setColor(r, g, b);
    }
}

void IntroScene::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN ||
        event.type == SDL_EVENT_FINGER_DOWN ||
        event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        requestReplace<LevelIntroScene>(1);
    }
}

void IntroScene::update(float deltaTime) {
    timer += deltaTime;

    // Update orbit positions and breathing for each block
    const float centerX = 320.0f;
    const float centerY = 120.0f;

    for (int i = 0; i < numBlocks; i++) {
        // Each block has different radius, speed, and starting angle
        float angle = timer * (0.8f + i * 0.2f) + (i * 3.14159f * 2.0f / numBlocks);
        float radius = 180.0f + (i % 2) * 40.0f;  // Alternate between two radii

        float x = centerX + std::cos(angle) * radius;
        float y = centerY + std::sin(angle) * radius * 0.5f;  // Squash for ellipse

        orbitBlocks[i].setPosition(x, y);
        orbitBlocks[i].update(deltaTime);
    }
}

void IntroScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 60, 255);
    SDL_RenderClear(renderer);

    // Render orbiting blocks (with breathing animation)
    for (int i = 0; i < numBlocks; i++) {
        orbitBlocks[i].render(renderer);
    }

    // Draw title (scaled up)
    SDL_SetRenderScale(renderer, 4.0f, 4.0f);
    SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);  // Yellow
    SDL_RenderDebugText(renderer, 55, 20, "MY GAME");
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // Draw subtitle
    SDL_SetRenderScale(renderer, 2.0f, 2.0f);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);  // Light gray
    SDL_RenderDebugText(renderer, 120, 80, "A Cool Adventure");
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // Draw "Press any key" with blinking effect
    int blink = (int)(timer * 2) % 2;  // Blink every 0.5 seconds
    if (blink == 0) {
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White
        SDL_RenderDebugText(renderer, 130, 250, "Press any key");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }
}

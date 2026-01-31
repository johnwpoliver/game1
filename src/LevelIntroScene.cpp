#include "LevelIntroScene.h"
#include "PlayingScene.h"
#include <cstdio>

void LevelIntroScene::onEnter() {
    SDL_Log("LevelIntroScene: Enter (Level %d)", level);
    timer = 0.0f;
}

void LevelIntroScene::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN ||
        event.type == SDL_EVENT_FINGER_DOWN ||
        event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        requestReplace<PlayingScene>(level);
    }
}

void LevelIntroScene::update(float deltaTime) {
    timer += deltaTime;
    // Timer available for animations, no auto-advance
}

void LevelIntroScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 40, 40, 80, 255);
    SDL_RenderClear(renderer);

    // Draw "Level X" title
    char levelText[32];
    snprintf(levelText, sizeof(levelText), "LEVEL %d", level);

    SDL_SetRenderScale(renderer, 4.0f, 4.0f);
    SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);  // Light blue
    SDL_RenderDebugText(renderer, 60, 30, levelText);
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // Draw "Get Ready!" subtitle
    SDL_SetRenderScale(renderer, 2.0f, 2.0f);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);  // Light gray
    SDL_RenderDebugText(renderer, 155, 100, "Get Ready!");
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // Draw "Press any key" with blinking effect
    int blink = (int)(timer * 2) % 2;
    if (blink == 0) {
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White
        SDL_RenderDebugText(renderer, 130, 250, "Press any key");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }
}

#include "PlayingScene.h"
#include "GameOverScene.h"
#include "Input.h"
#include "DisplayManager.h"

void PlayingScene::onEnter() {
    SDL_Log("PlayingScene: Enter (Level %d)", level);

    // Position UI elements
    lives.setPosition(10.0f, 10.0f);
    score.setPosition(DisplayManager::DESIGN_WIDTH - 10.0f, 10.0f);
}

void PlayingScene::onExit() {
    SDL_Log("PlayingScene: Exit");
}

void PlayingScene::handleEvent(const SDL_Event& event) {
    // Touch/mouse: left half = lose, right half = win (for testing)
    if (event.type == SDL_EVENT_FINGER_DOWN) {
        if (event.tfinger.x < 0.5f) {
            requestReplace<GameOverScene>(false); // lose
        } else {
            requestReplace<GameOverScene>(true);  // win
        }
    }
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        SDL_Window* window = SDL_GetWindowFromEvent(&event);
        if (window) {
            int windowWidth;
            SDL_GetWindowSize(window, &windowWidth, nullptr);
            if (event.button.x < windowWidth / 2) {
                requestReplace<GameOverScene>(false); // lose
            } else {
                requestReplace<GameOverScene>(true);  // win
            }
        }
    }
}

void PlayingScene::update(float deltaTime) {
    Input& input = Input::instance();

    // Check for back/escape (win for testing)
    if (input.justPressed(Action::Back)) {
        requestReplace<GameOverScene>(true);
        return;
    }

    // Handle movement using Input manager
    float dx = 0.0f, dy = 0.0f;
    if (input.isHeld(Action::MoveUp)) dy -= speed * deltaTime;
    if (input.isHeld(Action::MoveDown)) dy += speed * deltaTime;
    if (input.isHeld(Action::MoveLeft)) dx -= speed * deltaTime;
    if (input.isHeld(Action::MoveRight)) dx += speed * deltaTime;
    player.move(dx, dy);

    // Keep player within screen bounds
    player.clampToScreen(DisplayManager::DESIGN_WIDTH, DisplayManager::DESIGN_HEIGHT);

    // Update player animation
    player.update(deltaTime);
}

void PlayingScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderClear(renderer);

    player.render(renderer);

    // Draw UI
    lives.render(renderer);
    score.render(renderer);
}

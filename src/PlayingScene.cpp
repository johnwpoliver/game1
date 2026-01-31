#include "PlayingScene.h"
#include "GameOverScene.h"

void PlayingScene::onEnter() {
    SDL_Log("PlayingScene: Enter (Level %d)", level);
}

void PlayingScene::onExit() {
    SDL_Log("PlayingScene: Exit");
}

void PlayingScene::handleEvent(const SDL_Event& event) {



    if (event.type == SDL_EVENT_KEY_DOWN) {
        // ESC or Android back button = win
        if (event.key.scancode == SDL_SCANCODE_ESCAPE ||
            event.key.scancode == SDL_SCANCODE_AC_BACK) {
            requestReplace<GameOverScene>(true);  // win
        }
        // Space = lose
        if (event.key.scancode == SDL_SCANCODE_SPACE) {
            requestReplace<GameOverScene>(false); // lose
        }
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
        // ESC or Android back button = win
        if (event.key.scancode == SDL_SCANCODE_ESCAPE ||
            event.key.scancode == SDL_SCANCODE_AC_BACK) {
            requestReplace<GameOverScene>(true);  // win
        }
        // Space = lose
        if (event.key.scancode == SDL_SCANCODE_SPACE) {
            requestReplace<GameOverScene>(false); // lose
        }

        if (event.key.scancode == SDL_SCANCODE_UP) {
            // move up
        }
        if (event.key.scancode == SDL_SCANCODE_DOWN) {
            // move down
        }
        if (event.key.scancode == SDL_SCANCODE_LEFT) {
            // move left
        }
        if (event.key.scancode == SDL_SCANCODE_RIGHT) {
            // move right
        }
    }
    // Touch: left half = lose, right half = win
    if (event.type == SDL_EVENT_FINGER_DOWN) {
        if (event.tfinger.x < 0.5f) {
            requestReplace<GameOverScene>(false); // lose
        } else {
            requestReplace<GameOverScene>(true);  // win
        }
    }
    // Mouse click: left half = lose, right half = win
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
    // Handle movement input
    const bool* keys = SDL_GetKeyboardState(NULL);
    float dx = 0.0f, dy = 0.0f;
    if (keys[SDL_SCANCODE_UP]) dy -= speed * deltaTime;
    if (keys[SDL_SCANCODE_DOWN]) dy += speed * deltaTime;
    if (keys[SDL_SCANCODE_LEFT]) dx -= speed * deltaTime;
    if (keys[SDL_SCANCODE_RIGHT]) dx += speed * deltaTime;
    player.move(dx, dy);

    // Keep player within screen bounds
    player.clampToScreen(800.0f, 600.0f);

    // Update player animation
    player.update(deltaTime);
}

void PlayingScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderClear(renderer);

    player.render(renderer);
}

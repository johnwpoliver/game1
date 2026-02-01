#include "DisplayManager.h"

void DisplayManager::initialize(SDL_Window* window) {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    screenWidth = static_cast<float>(w);
    screenHeight = static_cast<float>(h);
    SDL_Log("DisplayManager: Initialized %.0fx%.0f", screenWidth, screenHeight);
}

void DisplayManager::handleResize(int newWidth, int newHeight) {
    screenWidth = static_cast<float>(newWidth);
    screenHeight = static_cast<float>(newHeight);
    SDL_Log("DisplayManager: Resized to %.0fx%.0f", screenWidth, screenHeight);
}

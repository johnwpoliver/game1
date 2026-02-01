#pragma once
#include <SDL3/SDL.h>

class DisplayManager {
public:
    static DisplayManager& instance() {
        static DisplayManager dm;
        return dm;
    }

    void initialize(SDL_Window* window);
    void handleResize(int newWidth, int newHeight);

    // Design dimensions (reference for all game logic)
    static constexpr float DESIGN_WIDTH = 800.0f;
    static constexpr float DESIGN_HEIGHT = 600.0f;

    float getScreenWidth() const { return screenWidth; }
    float getScreenHeight() const { return screenHeight; }

private:
    DisplayManager() = default;
    float screenWidth = DESIGN_WIDTH;
    float screenHeight = DESIGN_HEIGHT;
};

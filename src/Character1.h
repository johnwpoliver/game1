#pragma once
#include <SDL3/SDL.h>

class Character1 {
public:
    Character1(float startX = 100.0f, float startY = 100.0f);

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    void move(float dx, float dy);
    void setPosition(float x, float y);
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setSize(float size);
    void setBreathOffset(float offset);  // Start breathing at different phase
    void clampToScreen(float screenWidth, float screenHeight);

    float getX() const { return bottomCenterX; }
    float getY() const { return bottomY; }

private:
    float bottomCenterX;  // X position of bottom edge center
    float bottomY;        // Y position of bottom edge
    float baseSize;     // Base size of the square
    float breathTimer;  // Timer for breathing animation
    float breathSpeed;  // How fast the breathing cycle is
    float breathAmount; // How much the size varies (as a fraction of baseSize)

    // Color
    Uint8 r, g, b;
};

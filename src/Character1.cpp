#include "Character1.h"
#include <cmath>

Character1::Character1(float startX, float startY)
    : bottomCenterX(startX)
    , bottomY(startY)
    , baseSize(64.0f)
    , breathTimer(0.0f)
    , breathSpeed(3.0f)      // Complete cycle ~2 seconds
    , breathAmount(0.2f)     // Size varies by +/- 20%
    , r(255), g(100), b(100) // Red-ish color
{
}

void Character1::update(float deltaTime) {
    breathTimer += deltaTime * breathSpeed;
    // Keep timer from growing too large
    if (breathTimer > 6.28318f) {  // 2 * PI
        breathTimer -= 6.28318f;
    }
}

void Character1::render(SDL_Renderer* renderer) {
    // Calculate current size based on breathing animation
    // sin() gives -1 to 1, we want size to vary around baseSize
    float breathScale = 1.0f + std::sin(breathTimer) * breathAmount;
    float currentSize = baseSize * breathScale;

    // Calculate top-left corner so bottom-center stays fixed
    float x = bottomCenterX - currentSize / 2.0f;  // Center horizontally
    float y = bottomY - currentSize;               // Grow upward from bottom

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_FRect rect = {x, y, currentSize, currentSize};
    SDL_RenderFillRect(renderer, &rect);
}

void Character1::move(float dx, float dy) {
    bottomCenterX += dx;
    bottomY += dy;
}

void Character1::setPosition(float x, float y) {
    bottomCenterX = x;
    bottomY = y;
}

void Character1::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    r = red;
    g = green;
    b = blue;
}

void Character1::setSize(float size) {
    baseSize = size;
}

void Character1::setBreathOffset(float offset) {
    breathTimer = offset;
}

void Character1::clampToScreen(float screenWidth, float screenHeight) {
    // Account for max size during breathing animation
    float maxSize = baseSize * (1.0f + breathAmount);
    float halfWidth = maxSize / 2.0f;

    // Clamp horizontal (center X must keep square within bounds)
    if (bottomCenterX < halfWidth) {
        bottomCenterX = halfWidth;
    }
    if (bottomCenterX > screenWidth - halfWidth) {
        bottomCenterX = screenWidth - halfWidth;
    }

    // Clamp vertical (bottom Y, square grows upward)
    if (bottomY < maxSize) {
        bottomY = maxSize;
    }
    if (bottomY > screenHeight) {
        bottomY = screenHeight;
    }
}

void Character1::applyGravity(float deltaTime) {
    velocityY += gravity * deltaTime;
    bottomY += velocityY * deltaTime;
}

void Character1::jump() {
    if (grounded) {
        velocityY = jumpVelocity;
        grounded = false;
    }
}

void Character1::landOn(float groundY) {
    if (bottomY >= groundY) {
        bottomY = groundY;
        velocityY = 0.0f;
        grounded = true;
    } else {
        grounded = false;
    }
}

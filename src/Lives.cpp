#include "Lives.h"
#include <algorithm>
#include <cstdio>

Lives::Lives(int startingLives)
    : count(startingLives)
    , startCount(startingLives)
    , maxLives(startingLives + 2)  // Allow collecting up to 2 extra lives
{
}

void Lives::loseLife() {
    if (count > 0) {
        count--;
    }
}

void Lives::addLife() {
    if (count < maxLives) {
        count++;
    }
}

void Lives::reset() {
    count = startCount;
}

void Lives::render(SDL_Renderer* renderer) {
    // Draw heart icons for each life
    const float heartSize = 20.0f;
    const float spacing = 5.0f;

    for (int i = 0; i < count; i++) {
        float x = posX + i * (heartSize + spacing);
        float y = posY;

        // Draw a simple heart as a red square (placeholder for sprite)
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        SDL_FRect rect = {x, y, heartSize, heartSize};
        SDL_RenderFillRect(renderer, &rect);

        // Draw a small highlight
        SDL_SetRenderDrawColor(renderer, 255, 150, 150, 255);
        SDL_FRect highlight = {x + 2, y + 2, 6, 6};
        SDL_RenderFillRect(renderer, &highlight);
    }

    // Draw empty slots for lost lives
    for (int i = count; i < startCount; i++) {
        float x = posX + i * (heartSize + spacing);
        float y = posY;

        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_FRect rect = {x, y, heartSize, heartSize};
        SDL_RenderFillRect(renderer, &rect);
    }
}

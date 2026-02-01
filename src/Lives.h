#pragma once
#include <SDL3/SDL.h>

class Lives {
public:
    explicit Lives(int startingLives = 3);

    void loseLife();
    void addLife();
    void reset();

    int getCount() const { return count; }
    int getMax() const { return maxLives; }
    bool isGameOver() const { return count <= 0; }

    void setMax(int max) { maxLives = max; }
    void setPosition(float x, float y) { posX = x; posY = y; }

    void render(SDL_Renderer* renderer);

private:
    int count;
    int startCount;
    int maxLives;
    float posX = 10.0f;
    float posY = 10.0f;
};

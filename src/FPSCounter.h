#pragma once
#include <SDL3/SDL.h>

class FPSCounter {
public:
    void update(float deltaTime);

private:
    int frameCount = 0;
    float elapsedTime = 0.0f;
    float reportInterval = 5.0f;
};

#include "FPSCounter.h"

void FPSCounter::update(float deltaTime) {
    frameCount++;
    elapsedTime += deltaTime;

    if (elapsedTime >= reportInterval) {
        float fps = frameCount / elapsedTime;
        SDL_Log("FPS: %.1f (avg over %.0fs)", fps, elapsedTime);
        frameCount = 0;
        elapsedTime = 0.0f;
    }
}

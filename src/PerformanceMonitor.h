#pragma once
#include <SDL3/SDL.h>

class PerformanceMonitor {
public:
    void frameStart();
    void frameEnd();

private:
    Uint64 frameStartTime = 0;
    float totalProcessingTime = 0.0f;
    int frameCount = 0;
    float elapsedTime = 0.0f;
    float reportInterval = 5.0f;
};

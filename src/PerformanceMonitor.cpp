#include "PerformanceMonitor.h"

void PerformanceMonitor::frameStart() {
    frameStartTime = SDL_GetPerformanceCounter();
}

void PerformanceMonitor::frameEnd() {
    Uint64 frameEndTime = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();

    // Calculate processing time for this frame in seconds
    float processingTime = (float)(frameEndTime - frameStartTime) / (float)freq;
    totalProcessingTime += processingTime;
    frameCount++;
    elapsedTime += processingTime;

    // Also need to account for time waiting in VSync
    // We'll use a simple approximation based on last deltaTime
    static Uint64 lastFrameEnd = 0;
    if (lastFrameEnd > 0) {
        float totalFrameTime = (float)(frameEndTime - lastFrameEnd) / (float)freq;
        elapsedTime = elapsedTime - processingTime + totalFrameTime;
    }
    lastFrameEnd = frameEndTime;

    if (elapsedTime >= reportInterval) {
        float avgProcessingMs = (totalProcessingTime / frameCount) * 1000.0f;
        float vsyncIntervalMs = (elapsedTime / frameCount) * 1000.0f;
        float utilizationPercent = (totalProcessingTime / elapsedTime) * 100.0f;

        SDL_Log("Performance: %.2fms avg processing / %.2fms frame = %.1f%% utilization",
                avgProcessingMs, vsyncIntervalMs, utilizationPercent);

        totalProcessingTime = 0.0f;
        frameCount = 0;
        elapsedTime = 0.0f;
    }
}

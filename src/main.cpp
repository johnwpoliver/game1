#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "SceneManager.h"
#include "IntroScene.h"
#include "FPSCounter.h"
#include "PerformanceMonitor.h"

int main(int argc, char* argv[]) {
    SDL_Log("Starting game...");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }
    SDL_Log("SDL initialized");

    SDL_Window* window = SDL_CreateWindow("My Game", 800, 600, 0);
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Log("Renderer created");

    // Enable VSync for smooth rendering
    SDL_SetRenderVSync(renderer, 1);

    // Start with intro scene
    SDL_Log("Creating scene manager...");
    SceneManager& scenes = SceneManager::instance();
    SDL_Log("Pushing intro scene...");
    scenes.push(std::make_unique<IntroScene>());
    SDL_Log("Intro scene pushed");

    // Game loop timing
    Uint64 lastTime = SDL_GetTicks();
    FPSCounter fpsCounter;
    PerformanceMonitor perfMonitor;

    bool running = true;
    while (running && !scenes.isEmpty()) {
        perfMonitor.frameStart();
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // Cap delta time to avoid spiral of death
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            scenes.handleEvent(event);
        }

        // Update
        fpsCounter.update(deltaTime);
        scenes.update(deltaTime);

        // Render
        scenes.render(renderer);
        perfMonitor.frameEnd();
        SDL_RenderPresent(renderer);  // VSync will handle frame timing
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

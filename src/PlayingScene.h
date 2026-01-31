#pragma once
#include "SceneManager.h"
#include "Character1.h"

class PlayingScene : public Scene {
public:
    explicit PlayingScene(int levelNum = 1) : level(levelNum) {}

    void onEnter() override;
    void onExit() override;
    void handleEvent(const SDL_Event& event) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    int level;
    Character1 player{100.0f, 100.0f};
    float speed = 750.0f;
};

#pragma once
#include "SceneManager.h"

class LevelIntroScene : public Scene {
public:
    explicit LevelIntroScene(int levelNum = 1) : level(levelNum) {}

    void onEnter() override;
    void handleEvent(const SDL_Event& event) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    int level;
    float timer = 0.0f;
};

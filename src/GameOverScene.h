#pragma once
#include "SceneManager.h"
#include "Character1.h"
#include <array>

class GameOverScene : public Scene {
public:
    explicit GameOverScene(bool won, int finalScore = 0, int highScore = 0)
        : playerWon(won), finalScore(finalScore), highScore(highScore) {}

    void onEnter() override;
    void handleEvent(const SDL_Event& event) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    bool playerWon;
    int finalScore;
    int highScore;
    float timer = 0.0f;

    static constexpr int numBlocks = 20;
    std::array<Character1, numBlocks> blocks;
    std::array<float, numBlocks> velocityX;
    std::array<float, numBlocks> velocityY;
};

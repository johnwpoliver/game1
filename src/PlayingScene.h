#pragma once
#include "SceneManager.h"
#include "Character1.h"
#include "Lives.h"
#include "Score.h"
#include "Level.h"

class PlayingScene : public Scene {
public:
    explicit PlayingScene(int levelNum = 1) : levelNumber(levelNum) {}

    void onEnter() override;
    void onExit() override;
    void handleEvent(const SDL_Event& event) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    void loseLife();
    void checkCollisions();
    void renderLevel(SDL_Renderer* renderer);
    void restartLevel();

    int levelNumber;
    Level level;
    Character1 player{150.0f, 500.0f};

    // Auto-runner constants
    static constexpr float SCROLL_SPEED = 200.0f;
    static constexpr float PLAYER_X = 150.0f;
    static constexpr float PLAYER_SIZE = 64.0f;
    static constexpr float DEATH_PAUSE_DURATION = 1.0f;

    // Scrolling state
    float distanceTraveled = 0.0f;

    // Death pause state
    bool inDeathPause = false;
    float deathPauseTimer = 0.0f;
    bool gameOverPending = false;

    Lives lives{3};
    Score score;
};

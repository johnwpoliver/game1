#pragma once
#include <SDL3/SDL.h>
#include <string>

class Score {
public:
    Score() = default;

    void add(int points);
    void reset();

    int getValue() const { return value; }
    int getHighScore() const { return highScore; }

    void setPosition(float x, float y) { posX = x; posY = y; }
    void setScale(float s) { scale = s; }

    void loadHighScore(const std::string& filename = "highscore.dat");
    void saveHighScore(const std::string& filename = "highscore.dat");

    void render(SDL_Renderer* renderer);

private:
    int value = 0;
    int highScore = 0;
    float posX = 700.0f;  // Top right by default
    float posY = 10.0f;
    float scale = 2.0f;
};

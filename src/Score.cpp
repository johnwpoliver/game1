#include "Score.h"
#include <cstdio>
#include <algorithm>
#include <fstream>

void Score::add(int points) {
    value += points;
    if (value > highScore) {
        highScore = value;
    }
}

void Score::reset() {
    value = 0;
}

void Score::loadHighScore(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));
        if (!file) {
            highScore = 0;
        }
        SDL_Log("Score: Loaded high score: %d", highScore);
    } else {
        highScore = 0;
        SDL_Log("Score: No high score file found, starting fresh");
    }
}

void Score::saveHighScore(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&highScore), sizeof(highScore));
        SDL_Log("Score: Saved high score: %d", highScore);
    } else {
        SDL_Log("Score: Failed to save high score");
    }
}

void Score::render(SDL_Renderer* renderer) {
    const float labelScale = 1.5f;
    const float labelHeight = 8.0f * labelScale;

    // Draw "SCORE" label at top (right-aligned)
    float labelWidth = 5 * 8.0f * labelScale;  // "SCORE" is 5 chars
    float labelX = posX - labelWidth;

    SDL_SetRenderScale(renderer, labelScale, labelScale);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDebugText(renderer, labelX / labelScale, posY / labelScale, "SCORE");
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // Format score value
    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "%d", value);

    // Calculate text width (8 pixels per char at scale)
    int textLen = 0;
    for (const char* p = scoreText; *p; p++) textLen++;
    float textWidth = textLen * 8.0f * scale;

    // Draw score value below label (right-aligned)
    float scoreX = posX - textWidth;
    float scoreY = posY + labelHeight + 4.0f;  // Below the label with gap

    SDL_SetRenderScale(renderer, scale, scale);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, scoreX / scale, scoreY / scale, scoreText);
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
}

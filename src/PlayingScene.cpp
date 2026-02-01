#include "PlayingScene.h"
#include "GameOverScene.h"
#include "Input.h"
#include "DisplayManager.h"
#include <cmath>

void PlayingScene::onEnter() {
    SDL_Log("PlayingScene: Enter (Level %d)", levelNumber);

    // Load level file
    std::string levelPath = "assets/levels/level" + std::to_string(levelNumber) + ".json";
    if (!level.loadFromFile(levelPath)) {
        SDL_Log("PlayingScene: Failed to load level, using defaults");
    }

    // Load high score from file
    score.loadHighScore();

    // Position player at start
    player.setPosition(PLAYER_X, level.getGroundY());
    player.landOn(level.getGroundY());
    distanceTraveled = 0.0f;

    // Reset death pause state
    inDeathPause = false;
    deathPauseTimer = 0.0f;
    gameOverPending = false;

    // Position UI elements
    lives.setPosition(10.0f, 10.0f);
    score.setPosition(DisplayManager::DESIGN_WIDTH - 10.0f, 10.0f);
}

void PlayingScene::onExit() {
    SDL_Log("PlayingScene: Exit");
}

void PlayingScene::handleEvent(const SDL_Event& event) {
    // Input events handled by Input manager
}

void PlayingScene::update(float deltaTime) {
    // Handle death pause
    if (inDeathPause) {
        deathPauseTimer += deltaTime;
        if (deathPauseTimer >= DEATH_PAUSE_DURATION) {
            inDeathPause = false;
            deathPauseTimer = 0.0f;
            if (gameOverPending) {
                requestReplace<GameOverScene>(false, score.getValue(), score.getHighScore());
            } else {
                restartLevel();
            }
        }
        return;  // Don't update game during death pause
    }

    Input& input = Input::instance();

    // Check for back/escape
    if (input.justPressed(Action::Back)) {
        score.saveHighScore();
        requestReplace<GameOverScene>(true, score.getValue(), score.getHighScore());
        return;
    }

    // Jump input
    if (input.justPressed(Action::Jump)) {
        player.jump();
    }

    // Apply gravity
    player.applyGravity(deltaTime);

    // Calculate player's world X position (check both edges for landing)
    float playerWorldX = PLAYER_X + distanceTraveled;
    float halfSize = PLAYER_SIZE / 2.0f;
    float playerLeftX = playerWorldX - halfSize;
    float playerRightX = playerWorldX + halfSize;

    // Check for platform landing first (either edge touching counts)
    float platformYLeft = level.getPlatformSurfaceAt(playerLeftX, player.getY(), player.getVelocityY());
    float platformYRight = level.getPlatformSurfaceAt(playerRightX, player.getY(), player.getVelocityY());
    float platformY = (platformYLeft > 0) ? platformYLeft : platformYRight;

    if (platformY > 0) {
        player.landOn(platformY);
    }
    // Check for ground landing (either edge touching counts)
    else if (level.hasGroundAt(playerLeftX) || level.hasGroundAt(playerRightX)) {
        player.landOn(level.getGroundY());
    }
    // Falling through a gap (only fall if BOTH edges are over gap)
    else if (player.getY() > DisplayManager::DESIGN_HEIGHT) {
        loseLife();
        return;
    }

    // Check collisions with obstacles and treasures
    checkCollisions();

    // Auto-scroll
    distanceTraveled += SCROLL_SPEED * deltaTime;

    // Update score based on distance
    int distanceScore = static_cast<int>(distanceTraveled * 0.1f);
    if (distanceScore > score.getValue()) {
        score.add(distanceScore - score.getValue());
    }

    // Check level completion (player crosses finish line)
    float finishLineX = level.getLength();
    if (playerWorldX >= finishLineX) {
        SDL_Log("PlayingScene: Level %d complete!", levelNumber);
        score.saveHighScore();
        requestReplace<GameOverScene>(true, score.getValue(), score.getHighScore());
        return;
    }

    // Update player animation
    player.update(deltaTime);
}

void PlayingScene::checkCollisions() {
    float playerWorldX = PLAYER_X + distanceTraveled;
    float playerY = player.getY();
    float halfSize = PLAYER_SIZE / 2.0f;

    // Player bounding box in world coordinates
    float playerLeft = playerWorldX - halfSize;
    float playerRight = playerWorldX + halfSize;
    float playerTop = playerY - PLAYER_SIZE;
    float playerBottom = playerY;

    // Check obstacle collisions (AABB)
    for (const auto& obs : level.getObstacles()) {
        float obsRight = obs.x + obs.width;
        float obsBottom = obs.y + obs.height;

        if (playerLeft < obsRight && playerRight > obs.x &&
            playerTop < obsBottom && playerBottom > obs.y) {
            SDL_Log("PlayingScene: Hit obstacle at %.0f", obs.x);
            loseLife();
            return;
        }
    }

    // Check treasure collection
    float playerCenterY = playerY - PLAYER_SIZE / 2.0f;
    for (auto& treasure : level.getTreasures()) {
        if (treasure.collected) continue;

        float dx = playerWorldX - treasure.x;
        float dy = playerCenterY - treasure.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        float collectionRadius = PLAYER_SIZE / 2.0f + 15.0f;
        if (distance < collectionRadius) {
            treasure.collected = true;
            score.add(treasure.points);
            SDL_Log("PlayingScene: Collected treasure worth %d points", treasure.points);
        }
    }
}

void PlayingScene::loseLife() {
    lives.loseLife();
    inDeathPause = true;
    deathPauseTimer = 0.0f;

    if (lives.isGameOver()) {
        SDL_Log("PlayingScene: Game Over!");
        gameOverPending = true;
        score.saveHighScore();
    } else {
        SDL_Log("PlayingScene: Lost a life, restarting level. Lives remaining: %d", lives.getCount());
        gameOverPending = false;
    }
}

void PlayingScene::restartLevel() {
    level.reset();
    distanceTraveled = 0.0f;
    player.setPosition(PLAYER_X, level.getGroundY());
    player.landOn(level.getGroundY());
}

void PlayingScene::render(SDL_Renderer* renderer) {
    // During death pause, show black screen with message
    if (inDeathPause) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Show "OUCH!" or "GAME OVER" message
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        float scale = 3.0f;
        SDL_SetRenderScale(renderer, scale, scale);

        const char* message = gameOverPending ? "GAME OVER" : "OUCH!";
        float textWidth = strlen(message) * 8.0f;  // Approximate character width
        float x = (DisplayManager::DESIGN_WIDTH / scale - textWidth) / 2.0f;
        float y = DisplayManager::DESIGN_HEIGHT / scale / 2.0f - 8.0f;

        SDL_RenderDebugText(renderer, x, y, message);
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);

        // Show lives remaining if not game over
        if (!gameOverPending) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_SetRenderScale(renderer, 2.0f, 2.0f);
            char livesMsg[32];
            snprintf(livesMsg, sizeof(livesMsg), "Lives: %d", lives.getCount());
            float livesWidth = strlen(livesMsg) * 8.0f;
            SDL_RenderDebugText(renderer,
                (DisplayManager::DESIGN_WIDTH / 2.0f - livesWidth) / 2.0f,
                DisplayManager::DESIGN_HEIGHT / 2.0f / 2.0f + 20.0f,
                livesMsg);
            SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        }
        return;
    }

    // Normal rendering
    // Sky background
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderClear(renderer);

    // Render level elements
    renderLevel(renderer);

    // Draw player
    player.render(renderer);

    // Draw UI (on top)
    lives.render(renderer);
    score.render(renderer);
}

void PlayingScene::renderLevel(SDL_Renderer* renderer) {
    // Render ground segments
    SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);  // Forest green
    for (const auto& seg : level.getGround()) {
        float screenStartX = seg.startX - distanceTraveled;
        float screenEndX = seg.endX - distanceTraveled;

        // Only render if visible
        if (screenEndX > 0 && screenStartX < DisplayManager::DESIGN_WIDTH) {
            float visibleStart = std::max(0.0f, screenStartX);
            float visibleEnd = std::min(DisplayManager::DESIGN_WIDTH, screenEndX);
            SDL_FRect rect = {
                visibleStart,
                level.getGroundY(),
                visibleEnd - visibleStart,
                DisplayManager::DESIGN_HEIGHT - level.getGroundY()
            };
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Render platforms
    SDL_SetRenderDrawColor(renderer, 139, 90, 43, 255);  // Brown
    for (const auto& plat : level.getPlatforms()) {
        float screenX = plat.x - distanceTraveled;
        if (screenX + plat.width > 0 && screenX < DisplayManager::DESIGN_WIDTH) {
            SDL_FRect rect = {screenX, plat.y, plat.width, plat.height};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Render treasures
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);  // Gold
    for (const auto& treasure : level.getTreasures()) {
        if (treasure.collected) continue;

        float screenX = treasure.x - distanceTraveled;
        if (screenX > -20 && screenX < DisplayManager::DESIGN_WIDTH + 20) {
            float size = 20.0f;
            SDL_FRect rect = {screenX - size/2, treasure.y - size/2, size, size};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Render obstacles
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);  // Red
    for (const auto& obs : level.getObstacles()) {
        float screenX = obs.x - distanceTraveled;
        if (screenX + obs.width > 0 && screenX < DisplayManager::DESIGN_WIDTH) {
            SDL_FRect rect = {screenX, obs.y, obs.width, obs.height};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Render finish line (checkered flag pattern)
    float finishScreenX = level.getLength() - distanceTraveled;
    if (finishScreenX > -20 && finishScreenX < DisplayManager::DESIGN_WIDTH + 20) {
        const float flagWidth = 20.0f;
        const float squareSize = 20.0f;
        const int numSquares = 25;  // Full height coverage

        for (int i = 0; i < numSquares; i++) {
            float y = i * squareSize;

            // Left column
            SDL_SetRenderDrawColor(renderer, (i % 2 == 0) ? 255 : 0, (i % 2 == 0) ? 255 : 0, (i % 2 == 0) ? 255 : 0, 255);
            SDL_FRect left = {finishScreenX, y, flagWidth / 2, squareSize};
            SDL_RenderFillRect(renderer, &left);

            // Right column (opposite color)
            SDL_SetRenderDrawColor(renderer, (i % 2 == 1) ? 255 : 0, (i % 2 == 1) ? 255 : 0, (i % 2 == 1) ? 255 : 0, 255);
            SDL_FRect right = {finishScreenX + flagWidth / 2, y, flagWidth / 2, squareSize};
            SDL_RenderFillRect(renderer, &right);
        }

        // "FINISH" text above the flag
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow
        SDL_SetRenderScale(renderer, 1.5f, 1.5f);
        SDL_RenderDebugText(renderer, (finishScreenX - 20) / 1.5f, 60, "FINISH");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }
}

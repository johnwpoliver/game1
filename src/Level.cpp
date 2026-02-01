#include "Level.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <SDL3/SDL.h>

using json = nlohmann::json;

bool Level::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        SDL_Log("Level: Failed to open file: %s", path.c_str());
        return false;
    }

    try {
        json data = json::parse(file);

        name = data.value("name", "Unnamed Level");
        length = data.value("length", 2000.0f);
        groundY = data.value("groundY", 500.0f);

        // Parse ground segments
        ground.clear();
        if (data.contains("ground")) {
            for (const auto& seg : data["ground"]) {
                GroundSegment gs;
                gs.startX = seg.value("start", 0.0f);
                gs.endX = seg.value("end", 0.0f);
                ground.push_back(gs);
            }
        }

        // Parse platforms
        platforms.clear();
        if (data.contains("platforms")) {
            for (const auto& plat : data["platforms"]) {
                Platform p;
                p.x = plat.value("x", 0.0f);
                p.y = plat.value("y", 0.0f);
                p.width = plat.value("width", 100.0f);
                p.height = plat.value("height", 20.0f);
                platforms.push_back(p);
            }
        }

        // Parse treasures
        treasures.clear();
        if (data.contains("treasures")) {
            for (const auto& t : data["treasures"]) {
                Treasure tr;
                tr.x = t.value("x", 0.0f);
                tr.y = t.value("y", 0.0f);
                tr.points = t.value("points", 100);
                tr.collected = false;
                treasures.push_back(tr);
            }
        }

        // Parse obstacles
        obstacles.clear();
        if (data.contains("obstacles")) {
            for (const auto& o : data["obstacles"]) {
                Obstacle obs;
                obs.x = o.value("x", 0.0f);
                obs.y = o.value("y", 0.0f);
                obs.width = o.value("width", 30.0f);
                obs.height = o.value("height", 40.0f);
                obstacles.push_back(obs);
            }
        }

        // Store initial state for reset
        initialTreasures = treasures;

        SDL_Log("Level: Loaded '%s' - length: %.0f, ground segments: %zu, platforms: %zu, treasures: %zu, obstacles: %zu",
                name.c_str(), length, ground.size(), platforms.size(), treasures.size(), obstacles.size());

        return true;
    }
    catch (const json::exception& e) {
        SDL_Log("Level: JSON parse error: %s", e.what());
        return false;
    }
}

bool Level::hasGroundAt(float worldX) const {
    for (const auto& seg : ground) {
        if (worldX >= seg.startX && worldX <= seg.endX) {
            return true;
        }
    }
    return false;
}

float Level::getPlatformSurfaceAt(float worldX, float playerBottomY, float velocityY) const {
    // Only land on platforms when falling (positive velocity = moving down)
    if (velocityY < 0.0f) {
        return -1.0f;  // Going up, don't land
    }

    for (const auto& plat : platforms) {
        // Check horizontal overlap
        if (worldX >= plat.x && worldX <= plat.x + plat.width) {
            // Check if player is near platform top (within landing tolerance)
            float landingTolerance = 15.0f;  // Pixels of tolerance for landing
            if (playerBottomY >= plat.y && playerBottomY <= plat.y + landingTolerance) {
                return plat.y;
            }
        }
    }
    return -1.0f;
}

void Level::reset() {
    treasures = initialTreasures;
}

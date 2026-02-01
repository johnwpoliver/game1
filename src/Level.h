#pragma once
#include <string>
#include <vector>

struct GroundSegment {
    float startX;
    float endX;
};

struct Platform {
    float x;
    float y;
    float width;
    float height;
};

struct Treasure {
    float x;
    float y;
    int points;
    bool collected = false;
};

struct Obstacle {
    float x;
    float y;
    float width;
    float height;
};

class Level {
public:
    bool loadFromFile(const std::string& path);

    float getLength() const { return length; }
    float getGroundY() const { return groundY; }
    const std::string& getName() const { return name; }

    bool hasGroundAt(float worldX) const;
    float getPlatformSurfaceAt(float worldX, float playerBottomY, float velocityY) const;

    const std::vector<GroundSegment>& getGround() const { return ground; }
    const std::vector<Platform>& getPlatforms() const { return platforms; }
    std::vector<Treasure>& getTreasures() { return treasures; }
    const std::vector<Obstacle>& getObstacles() const { return obstacles; }

    void reset();

private:
    std::string name;
    float length = 0.0f;
    float groundY = 500.0f;
    std::vector<GroundSegment> ground;
    std::vector<Platform> platforms;
    std::vector<Treasure> treasures;
    std::vector<Obstacle> obstacles;

    std::vector<Treasure> initialTreasures;  // For reset
};

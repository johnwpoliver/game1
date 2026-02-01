#include <gtest/gtest.h>
#include "Level.h"
#include <fstream>

class LevelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a test level JSON file
        std::ofstream file("test_level.json");
        file << R"({
            "name": "Test Level",
            "length": 2000,
            "groundY": 500,
            "ground": [
                {"start": 0, "end": 500},
                {"start": 600, "end": 1000},
                {"start": 1200, "end": 2000}
            ],
            "platforms": [
                {"x": 300, "y": 400, "width": 100, "height": 20},
                {"x": 800, "y": 350, "width": 150, "height": 20}
            ],
            "treasures": [
                {"x": 350, "y": 370, "points": 100},
                {"x": 875, "y": 320, "points": 50}
            ],
            "obstacles": [
                {"x": 450, "y": 460, "width": 30, "height": 40},
                {"x": 900, "y": 460, "width": 40, "height": 40}
            ]
        })";
        file.close();
    }

    void TearDown() override {
        std::remove("test_level.json");
    }
};

TEST_F(LevelTest, LoadFromFile) {
    Level level;
    EXPECT_TRUE(level.loadFromFile("test_level.json"));
    EXPECT_EQ(level.getName(), "Test Level");
    EXPECT_FLOAT_EQ(level.getLength(), 2000.0f);
    EXPECT_FLOAT_EQ(level.getGroundY(), 500.0f);
}

TEST_F(LevelTest, LoadNonExistentFile) {
    Level level;
    EXPECT_FALSE(level.loadFromFile("nonexistent.json"));
}

TEST_F(LevelTest, GroundSegments) {
    Level level;
    level.loadFromFile("test_level.json");

    const auto& ground = level.getGround();
    EXPECT_EQ(ground.size(), 3);
    EXPECT_FLOAT_EQ(ground[0].startX, 0.0f);
    EXPECT_FLOAT_EQ(ground[0].endX, 500.0f);
    EXPECT_FLOAT_EQ(ground[1].startX, 600.0f);
    EXPECT_FLOAT_EQ(ground[1].endX, 1000.0f);
}

TEST_F(LevelTest, Platforms) {
    Level level;
    level.loadFromFile("test_level.json");

    const auto& platforms = level.getPlatforms();
    EXPECT_EQ(platforms.size(), 2);
    EXPECT_FLOAT_EQ(platforms[0].x, 300.0f);
    EXPECT_FLOAT_EQ(platforms[0].y, 400.0f);
    EXPECT_FLOAT_EQ(platforms[0].width, 100.0f);
    EXPECT_FLOAT_EQ(platforms[0].height, 20.0f);
}

TEST_F(LevelTest, Treasures) {
    Level level;
    level.loadFromFile("test_level.json");

    auto& treasures = level.getTreasures();
    EXPECT_EQ(treasures.size(), 2);
    EXPECT_FLOAT_EQ(treasures[0].x, 350.0f);
    EXPECT_FLOAT_EQ(treasures[0].y, 370.0f);
    EXPECT_EQ(treasures[0].points, 100);
    EXPECT_FALSE(treasures[0].collected);
}

TEST_F(LevelTest, Obstacles) {
    Level level;
    level.loadFromFile("test_level.json");

    const auto& obstacles = level.getObstacles();
    EXPECT_EQ(obstacles.size(), 2);
    EXPECT_FLOAT_EQ(obstacles[0].x, 450.0f);
    EXPECT_FLOAT_EQ(obstacles[0].y, 460.0f);
    EXPECT_FLOAT_EQ(obstacles[0].width, 30.0f);
    EXPECT_FLOAT_EQ(obstacles[0].height, 40.0f);
}

// Ground collision tests
class GroundCollisionTest : public LevelTest {};

TEST_F(GroundCollisionTest, HasGroundAtStart) {
    Level level;
    level.loadFromFile("test_level.json");

    EXPECT_TRUE(level.hasGroundAt(0.0f));
    EXPECT_TRUE(level.hasGroundAt(250.0f));
    EXPECT_TRUE(level.hasGroundAt(500.0f));
}

TEST_F(GroundCollisionTest, NoGroundInGap) {
    Level level;
    level.loadFromFile("test_level.json");

    // Gap between 500 and 600
    EXPECT_FALSE(level.hasGroundAt(550.0f));
    // Gap between 1000 and 1200
    EXPECT_FALSE(level.hasGroundAt(1100.0f));
}

TEST_F(GroundCollisionTest, HasGroundAtBoundary) {
    Level level;
    level.loadFromFile("test_level.json");

    // Exactly at segment boundaries
    EXPECT_TRUE(level.hasGroundAt(500.0f));   // End of first segment
    EXPECT_TRUE(level.hasGroundAt(600.0f));   // Start of second segment
    EXPECT_TRUE(level.hasGroundAt(1000.0f));  // End of second segment
    EXPECT_TRUE(level.hasGroundAt(1200.0f));  // Start of third segment
}

// Platform collision tests
class PlatformCollisionTest : public LevelTest {};

TEST_F(PlatformCollisionTest, LandOnPlatformWhenFalling) {
    Level level;
    level.loadFromFile("test_level.json");

    // Platform at x=300-400, y=400
    // Player falling through platform area
    float platformY = level.getPlatformSurfaceAt(350.0f, 405.0f, 100.0f);  // Falling (positive velocity)
    EXPECT_FLOAT_EQ(platformY, 400.0f);
}

TEST_F(PlatformCollisionTest, NoLandWhenJumping) {
    Level level;
    level.loadFromFile("test_level.json");

    // Player jumping up through platform
    float platformY = level.getPlatformSurfaceAt(350.0f, 405.0f, -100.0f);  // Jumping (negative velocity)
    EXPECT_FLOAT_EQ(platformY, -1.0f);  // No landing when going up
}

TEST_F(PlatformCollisionTest, NoLandWhenNotOnPlatform) {
    Level level;
    level.loadFromFile("test_level.json");

    // X position not on any platform
    float platformY = level.getPlatformSurfaceAt(200.0f, 405.0f, 100.0f);
    EXPECT_FLOAT_EQ(platformY, -1.0f);
}

TEST_F(PlatformCollisionTest, NoLandWhenTooFarAbove) {
    Level level;
    level.loadFromFile("test_level.json");

    // Player too far above platform
    float platformY = level.getPlatformSurfaceAt(350.0f, 300.0f, 100.0f);
    EXPECT_FLOAT_EQ(platformY, -1.0f);
}

// Reset tests
class ResetTest : public LevelTest {};

TEST_F(ResetTest, ResetRestoresTreasures) {
    Level level;
    level.loadFromFile("test_level.json");

    // Collect first treasure
    auto& treasures = level.getTreasures();
    treasures[0].collected = true;
    treasures[1].collected = true;

    EXPECT_TRUE(treasures[0].collected);
    EXPECT_TRUE(treasures[1].collected);

    // Reset should restore treasures
    level.reset();

    EXPECT_FALSE(level.getTreasures()[0].collected);
    EXPECT_FALSE(level.getTreasures()[1].collected);
}

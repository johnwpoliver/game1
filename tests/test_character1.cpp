#include <gtest/gtest.h>
#include "Character1.h"

class Character1Test : public ::testing::Test {
protected:
    void SetUp() override {
        character = new Character1(100.0f, 200.0f);
    }

    void TearDown() override {
        delete character;
    }

    Character1* character;
};

TEST_F(Character1Test, ConstructorSetsPosition) {
    EXPECT_FLOAT_EQ(character->getX(), 100.0f);
    EXPECT_FLOAT_EQ(character->getY(), 200.0f);
}

TEST_F(Character1Test, DefaultConstructorPosition) {
    Character1 defaultChar;
    EXPECT_FLOAT_EQ(defaultChar.getX(), 100.0f);
    EXPECT_FLOAT_EQ(defaultChar.getY(), 100.0f);
}

TEST_F(Character1Test, MoveAddsToPosition) {
    character->move(10.0f, 20.0f);
    EXPECT_FLOAT_EQ(character->getX(), 110.0f);
    EXPECT_FLOAT_EQ(character->getY(), 220.0f);
}

TEST_F(Character1Test, MoveNegativeValues) {
    character->move(-50.0f, -100.0f);
    EXPECT_FLOAT_EQ(character->getX(), 50.0f);
    EXPECT_FLOAT_EQ(character->getY(), 100.0f);
}

TEST_F(Character1Test, SetPositionOverwrites) {
    character->setPosition(500.0f, 300.0f);
    EXPECT_FLOAT_EQ(character->getX(), 500.0f);
    EXPECT_FLOAT_EQ(character->getY(), 300.0f);
}

// clampToScreen tests - the most complex logic
class ClampToScreenTest : public ::testing::Test {
protected:
    void SetUp() override {
        character = new Character1(400.0f, 300.0f);
        // Default baseSize is 64, breathAmount is 0.2
        // So maxSize = 64 * 1.2 = 76.8
        // halfWidth = 38.4
    }

    void TearDown() override {
        delete character;
    }

    Character1* character;
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;
};

TEST_F(ClampToScreenTest, CenterPositionUnchanged) {
    // Character at center should not be clamped
    character->clampToScreen(screenWidth, screenHeight);
    EXPECT_FLOAT_EQ(character->getX(), 400.0f);
    EXPECT_FLOAT_EQ(character->getY(), 300.0f);
}

TEST_F(ClampToScreenTest, ClampLeftEdge) {
    character->setPosition(10.0f, 300.0f);  // Too far left
    character->clampToScreen(screenWidth, screenHeight);
    // Should clamp to halfWidth (38.4)
    EXPECT_GT(character->getX(), 10.0f);
    EXPECT_NEAR(character->getX(), 38.4f, 0.1f);
}

TEST_F(ClampToScreenTest, ClampRightEdge) {
    character->setPosition(790.0f, 300.0f);  // Too far right
    character->clampToScreen(screenWidth, screenHeight);
    // Should clamp to screenWidth - halfWidth (800 - 38.4 = 761.6)
    EXPECT_LT(character->getX(), 790.0f);
    EXPECT_NEAR(character->getX(), 761.6f, 0.1f);
}

TEST_F(ClampToScreenTest, ClampTopEdge) {
    character->setPosition(400.0f, 50.0f);  // Too high (Y too small)
    character->clampToScreen(screenWidth, screenHeight);
    // Should clamp to maxSize (76.8)
    EXPECT_GT(character->getY(), 50.0f);
    EXPECT_NEAR(character->getY(), 76.8f, 0.1f);
}

TEST_F(ClampToScreenTest, ClampBottomEdge) {
    character->setPosition(400.0f, 650.0f);  // Below screen
    character->clampToScreen(screenWidth, screenHeight);
    // Should clamp to screenHeight (600)
    EXPECT_FLOAT_EQ(character->getY(), 600.0f);
}

TEST_F(ClampToScreenTest, ClampCorner) {
    character->setPosition(-100.0f, -100.0f);  // Way outside
    character->clampToScreen(screenWidth, screenHeight);
    EXPECT_NEAR(character->getX(), 38.4f, 0.1f);
    EXPECT_NEAR(character->getY(), 76.8f, 0.1f);
}

TEST_F(ClampToScreenTest, CustomSizeAffectsClamping) {
    character->setSize(100.0f);  // Larger character
    // maxSize = 100 * 1.2 = 120, halfWidth = 60
    character->setPosition(30.0f, 300.0f);
    character->clampToScreen(screenWidth, screenHeight);
    EXPECT_NEAR(character->getX(), 60.0f, 0.1f);
}

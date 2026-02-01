#include <gtest/gtest.h>
#include "DisplayManager.h"

TEST(DisplayManagerTest, DesignConstants) {
    EXPECT_FLOAT_EQ(DisplayManager::DESIGN_WIDTH, 800.0f);
    EXPECT_FLOAT_EQ(DisplayManager::DESIGN_HEIGHT, 600.0f);
}

TEST(DisplayManagerTest, SingletonReturnsSameInstance) {
    DisplayManager& dm1 = DisplayManager::instance();
    DisplayManager& dm2 = DisplayManager::instance();
    EXPECT_EQ(&dm1, &dm2);
}

TEST(DisplayManagerTest, DefaultScreenDimensions) {
    DisplayManager& dm = DisplayManager::instance();
    // Default should match design dimensions
    EXPECT_FLOAT_EQ(dm.getScreenWidth(), DisplayManager::DESIGN_WIDTH);
    EXPECT_FLOAT_EQ(dm.getScreenHeight(), DisplayManager::DESIGN_HEIGHT);
}

TEST(DisplayManagerTest, HandleResizeUpdatesDimensions) {
    DisplayManager& dm = DisplayManager::instance();

    dm.handleResize(1920, 1080);
    EXPECT_FLOAT_EQ(dm.getScreenWidth(), 1920.0f);
    EXPECT_FLOAT_EQ(dm.getScreenHeight(), 1080.0f);

    dm.handleResize(1280, 720);
    EXPECT_FLOAT_EQ(dm.getScreenWidth(), 1280.0f);
    EXPECT_FLOAT_EQ(dm.getScreenHeight(), 720.0f);

    // Reset to default for other tests
    dm.handleResize(800, 600);
}

TEST(DisplayManagerTest, AspectRatioCalculation) {
    // 4:3 aspect ratio (design)
    float designAspect = DisplayManager::DESIGN_WIDTH / DisplayManager::DESIGN_HEIGHT;
    EXPECT_NEAR(designAspect, 4.0f / 3.0f, 0.001f);
}

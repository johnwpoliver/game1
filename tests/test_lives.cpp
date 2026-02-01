#include <gtest/gtest.h>
#include "Lives.h"

TEST(LivesTest, DefaultStartsWithThreeLives) {
    Lives lives;
    EXPECT_EQ(lives.getCount(), 3);
}

TEST(LivesTest, CustomStartingLives) {
    Lives lives(5);
    EXPECT_EQ(lives.getCount(), 5);
}

TEST(LivesTest, LoseLifeDecrementsCount) {
    Lives lives(3);
    lives.loseLife();
    EXPECT_EQ(lives.getCount(), 2);
}

TEST(LivesTest, LoseAllLives) {
    Lives lives(3);
    lives.loseLife();
    lives.loseLife();
    lives.loseLife();
    EXPECT_EQ(lives.getCount(), 0);
}

TEST(LivesTest, CannotGoNegative) {
    Lives lives(1);
    lives.loseLife();
    lives.loseLife();  // Try to go negative
    EXPECT_EQ(lives.getCount(), 0);
}

TEST(LivesTest, IsGameOverWhenNoLives) {
    Lives lives(1);
    EXPECT_FALSE(lives.isGameOver());
    lives.loseLife();
    EXPECT_TRUE(lives.isGameOver());
}

TEST(LivesTest, AddLifeIncrementsCount) {
    Lives lives(3);
    lives.loseLife();
    EXPECT_EQ(lives.getCount(), 2);
    lives.addLife();
    EXPECT_EQ(lives.getCount(), 3);
}

TEST(LivesTest, CannotExceedMaxLives) {
    Lives lives(3);
    lives.addLife();
    lives.addLife();
    lives.addLife();  // Should hit max (default is startCount + 2 = 5)
    EXPECT_LE(lives.getCount(), lives.getMax());
}

TEST(LivesTest, ResetRestoresToStartingLives) {
    Lives lives(3);
    lives.loseLife();
    lives.loseLife();
    EXPECT_EQ(lives.getCount(), 1);
    lives.reset();
    EXPECT_EQ(lives.getCount(), 3);
}

TEST(LivesTest, SetMaxLives) {
    Lives lives(3);
    lives.setMax(10);
    EXPECT_EQ(lives.getMax(), 10);

    // Should now be able to add more lives
    for (int i = 0; i < 7; i++) {
        lives.addLife();
    }
    EXPECT_EQ(lives.getCount(), 10);
}

TEST(LivesTest, SetPosition) {
    Lives lives;
    lives.setPosition(100.0f, 200.0f);
    // Position is used in render - just verify no crash
    SUCCEED();
}

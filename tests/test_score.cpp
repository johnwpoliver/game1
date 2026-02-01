#include <gtest/gtest.h>
#include "Score.h"

TEST(ScoreTest, StartsAtZero) {
    Score score;
    EXPECT_EQ(score.getValue(), 0);
}

TEST(ScoreTest, AddPoints) {
    Score score;
    score.add(100);
    EXPECT_EQ(score.getValue(), 100);
}

TEST(ScoreTest, AddMultipleTimes) {
    Score score;
    score.add(100);
    score.add(50);
    score.add(25);
    EXPECT_EQ(score.getValue(), 175);
}

TEST(ScoreTest, ResetClearsScore) {
    Score score;
    score.add(500);
    EXPECT_EQ(score.getValue(), 500);
    score.reset();
    EXPECT_EQ(score.getValue(), 0);
}

TEST(ScoreTest, HighScoreTracked) {
    Score score;
    score.add(100);
    EXPECT_EQ(score.getHighScore(), 100);

    score.add(200);
    EXPECT_EQ(score.getHighScore(), 300);
}

TEST(ScoreTest, HighScorePreservedAfterReset) {
    Score score;
    score.add(500);
    EXPECT_EQ(score.getHighScore(), 500);

    score.reset();
    EXPECT_EQ(score.getValue(), 0);
    EXPECT_EQ(score.getHighScore(), 500);  // High score should persist
}

TEST(ScoreTest, HighScoreUpdatesOnNewHigh) {
    Score score;
    score.add(100);
    score.reset();
    score.add(200);  // New high score

    EXPECT_EQ(score.getValue(), 200);
    EXPECT_EQ(score.getHighScore(), 200);
}

TEST(ScoreTest, HighScoreDoesNotDecreaseOnLowerScore) {
    Score score;
    score.add(500);
    score.reset();
    score.add(100);

    EXPECT_EQ(score.getValue(), 100);
    EXPECT_EQ(score.getHighScore(), 500);  // Should still be 500
}

TEST(ScoreTest, SetPosition) {
    Score score;
    score.setPosition(400.0f, 50.0f);
    SUCCEED();
}

TEST(ScoreTest, SetScale) {
    Score score;
    score.setScale(3.0f);
    SUCCEED();
}

TEST(ScoreTest, AddNegativePointsAllowed) {
    // Some games have penalties
    Score score;
    score.add(100);
    score.add(-50);
    EXPECT_EQ(score.getValue(), 50);
}

TEST(ScoreTest, LargeScore) {
    Score score;
    score.add(1000000);
    score.add(2000000);
    EXPECT_EQ(score.getValue(), 3000000);
}

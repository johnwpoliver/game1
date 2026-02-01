#include <gtest/gtest.h>
#include "SceneManager.h"
#include <string>
#include <vector>

// Mock scene that tracks lifecycle calls
class MockScene : public Scene {
public:
    static std::vector<std::string> events;
    std::string name;

    MockScene(const std::string& sceneName) : name(sceneName) {}

    void onEnter() override { events.push_back(name + ":onEnter"); }
    void onExit() override { events.push_back(name + ":onExit"); }
    void onPause() override { events.push_back(name + ":onPause"); }
    void onResume() override { events.push_back(name + ":onResume"); }
    void update(float deltaTime) override { events.push_back(name + ":update"); }
    void render(SDL_Renderer* renderer) override { events.push_back(name + ":render"); }
};

std::vector<std::string> MockScene::events;

class SceneManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        MockScene::events.clear();
        // Clear any existing scenes by popping until empty
        SceneManager& sm = SceneManager::instance();
        while (!sm.isEmpty()) {
            sm.pop();
            sm.update(0.0f);  // Process the pop
        }
        MockScene::events.clear();  // Clear events from cleanup
    }

    void TearDown() override {
        // Clean up any remaining scenes
        SceneManager& sm = SceneManager::instance();
        while (!sm.isEmpty()) {
            sm.pop();
            sm.update(0.0f);
        }
    }
};

TEST_F(SceneManagerTest, InitiallyEmpty) {
    SceneManager& sm = SceneManager::instance();
    EXPECT_TRUE(sm.isEmpty());
    EXPECT_EQ(sm.current(), nullptr);
}

TEST_F(SceneManagerTest, PushSingleScene) {
    SceneManager& sm = SceneManager::instance();

    sm.push(std::make_unique<MockScene>("A"));
    // Scene not yet active until update processes pending
    EXPECT_EQ(sm.current(), nullptr);

    sm.update(0.0f);  // Process pending push

    EXPECT_FALSE(sm.isEmpty());
    EXPECT_NE(sm.current(), nullptr);

    // Check lifecycle: onEnter should be called
    ASSERT_GE(MockScene::events.size(), 1);
    EXPECT_EQ(MockScene::events[0], "A:onEnter");
}

TEST_F(SceneManagerTest, PushMultipleScenes) {
    SceneManager& sm = SceneManager::instance();

    sm.push(std::make_unique<MockScene>("A"));
    sm.update(0.0f);
    MockScene::events.clear();

    sm.push(std::make_unique<MockScene>("B"));
    sm.update(0.0f);

    // A should be paused, B should enter
    ASSERT_GE(MockScene::events.size(), 2);
    EXPECT_EQ(MockScene::events[0], "A:onPause");
    EXPECT_EQ(MockScene::events[1], "B:onEnter");
}

TEST_F(SceneManagerTest, PopScene) {
    SceneManager& sm = SceneManager::instance();

    sm.push(std::make_unique<MockScene>("A"));
    sm.update(0.0f);
    sm.push(std::make_unique<MockScene>("B"));
    sm.update(0.0f);
    MockScene::events.clear();

    sm.pop();
    sm.update(0.0f);

    // B should exit, A should resume
    ASSERT_GE(MockScene::events.size(), 2);
    EXPECT_EQ(MockScene::events[0], "B:onExit");
    EXPECT_EQ(MockScene::events[1], "A:onResume");
}

TEST_F(SceneManagerTest, PopLastScene) {
    SceneManager& sm = SceneManager::instance();

    sm.push(std::make_unique<MockScene>("A"));
    sm.update(0.0f);
    MockScene::events.clear();

    sm.pop();
    sm.update(0.0f);

    EXPECT_TRUE(sm.isEmpty());
    EXPECT_EQ(sm.current(), nullptr);

    // A should exit, no resume since it was the last scene
    ASSERT_GE(MockScene::events.size(), 1);
    EXPECT_EQ(MockScene::events[0], "A:onExit");
}

TEST_F(SceneManagerTest, ReplaceScene) {
    SceneManager& sm = SceneManager::instance();

    sm.push(std::make_unique<MockScene>("A"));
    sm.update(0.0f);
    MockScene::events.clear();

    sm.replace(std::make_unique<MockScene>("B"));
    sm.update(0.0f);

    EXPECT_FALSE(sm.isEmpty());

    // A should exit, B should enter (no pause since it's a replace)
    ASSERT_GE(MockScene::events.size(), 2);
    EXPECT_EQ(MockScene::events[0], "A:onExit");
    EXPECT_EQ(MockScene::events[1], "B:onEnter");
}

TEST_F(SceneManagerTest, SingletonReturnsSameInstance) {
    SceneManager& sm1 = SceneManager::instance();
    SceneManager& sm2 = SceneManager::instance();
    EXPECT_EQ(&sm1, &sm2);
}

TEST_F(SceneManagerTest, CurrentReturnsTopScene) {
    SceneManager& sm = SceneManager::instance();

    sm.push(std::make_unique<MockScene>("A"));
    sm.update(0.0f);

    sm.push(std::make_unique<MockScene>("B"));
    sm.update(0.0f);

    // current() should return B (the top of the stack)
    EXPECT_NE(sm.current(), nullptr);
}

TEST_F(SceneManagerTest, UpdateCallsCurrentSceneUpdate) {
    SceneManager& sm = SceneManager::instance();

    sm.push(std::make_unique<MockScene>("A"));
    sm.update(0.0f);  // Process push
    MockScene::events.clear();

    sm.update(0.016f);  // Regular update

    // Should see update called
    ASSERT_GE(MockScene::events.size(), 1);
    EXPECT_EQ(MockScene::events[0], "A:update");
}

TEST_F(SceneManagerTest, RenderCallsAllScenes) {
    SceneManager& sm = SceneManager::instance();

    sm.push(std::make_unique<MockScene>("A"));
    sm.update(0.0f);
    sm.push(std::make_unique<MockScene>("B"));
    sm.update(0.0f);
    MockScene::events.clear();

    sm.render(nullptr);

    // Both scenes should be rendered (for overlay support)
    ASSERT_GE(MockScene::events.size(), 2);
    EXPECT_EQ(MockScene::events[0], "A:render");
    EXPECT_EQ(MockScene::events[1], "B:render");
}

TEST_F(SceneManagerTest, PendingOperationsProcessedInOrder) {
    SceneManager& sm = SceneManager::instance();

    // Queue multiple operations before update
    sm.push(std::make_unique<MockScene>("A"));
    sm.push(std::make_unique<MockScene>("B"));
    sm.update(0.0f);

    // Both should be processed: A enters, A pauses, B enters
    ASSERT_GE(MockScene::events.size(), 3);
    EXPECT_EQ(MockScene::events[0], "A:onEnter");
    EXPECT_EQ(MockScene::events[1], "A:onPause");
    EXPECT_EQ(MockScene::events[2], "B:onEnter");
}

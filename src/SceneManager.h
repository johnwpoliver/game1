#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>

class Scene;

class SceneManager {
public:
    static SceneManager& instance() {
        static SceneManager mgr;
        return mgr;
    }

    void push(std::unique_ptr<Scene> scene);
    void pop();
    void replace(std::unique_ptr<Scene> scene);

    void handleEvent(const SDL_Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    bool isEmpty() const { return scenes.empty() && pendingPush.empty() && !pendingReplace; }
    Scene* current() const { return scenes.empty() ? nullptr : scenes.back().get(); }

private:
    SceneManager() = default;
    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<std::unique_ptr<Scene>> pendingPush;
    int pendingPop = 0;
    std::unique_ptr<Scene> pendingReplace;

    void processPending();
};

class Scene {
public:
    virtual ~Scene() = default;

    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onPause() {}
    virtual void onResume() {}

    virtual void handleEvent(const SDL_Event& event) {}
    virtual void update(float deltaTime) {}
    virtual void render(SDL_Renderer* renderer) = 0;

    void requestPop() { SceneManager::instance().pop(); }

    template<typename T, typename... Args>
    void requestPush(Args&&... args) {
        SceneManager::instance().push(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<typename T, typename... Args>
    void requestReplace(Args&&... args) {
        SceneManager::instance().replace(std::make_unique<T>(std::forward<Args>(args)...));
    }
};

// Implementation
inline void SceneManager::push(std::unique_ptr<Scene> scene) {
    pendingPush.push_back(std::move(scene));
}

inline void SceneManager::pop() {
    pendingPop++;
}

inline void SceneManager::replace(std::unique_ptr<Scene> scene) {
    pendingReplace = std::move(scene);
}

inline void SceneManager::processPending() {
    // Handle replace first
    if (pendingReplace) {
        if (!scenes.empty()) {
            scenes.back()->onExit();
            scenes.pop_back();
        }
        pendingReplace->onEnter();
        scenes.push_back(std::move(pendingReplace));
        pendingReplace = nullptr;
    }

    // Handle pops
    while (pendingPop > 0 && !scenes.empty()) {
        scenes.back()->onExit();
        scenes.pop_back();
        if (!scenes.empty()) {
            scenes.back()->onResume();
        }
        pendingPop--;
    }
    pendingPop = 0;

    // Handle pushes
    for (auto& scene : pendingPush) {
        if (!scenes.empty()) {
            scenes.back()->onPause();
        }
        scene->onEnter();
        scenes.push_back(std::move(scene));
    }
    pendingPush.clear();
}

inline void SceneManager::handleEvent(const SDL_Event& event) {
    if (!scenes.empty()) {
        scenes.back()->handleEvent(event);
    }
}

inline void SceneManager::update(float deltaTime) {
    processPending();
    if (!scenes.empty()) {
        scenes.back()->update(deltaTime);
    }
}

inline void SceneManager::render(SDL_Renderer* renderer) {
    // Render all scenes (allows transparency/overlay)
    for (auto& scene : scenes) {
        scene->render(renderer);
    }
}

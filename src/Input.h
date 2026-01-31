#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>

enum class Action {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Confirm,      // Space, Enter, or tap
    Back,         // Escape, Android back
    Pause
};

class Input {
public:
    static Input& instance() {
        static Input input;
        return input;
    }

    // Call once per frame before polling events
    void beginFrame();

    // Call for each SDL event
    void processEvent(const SDL_Event& event);

    // Query input state
    bool isHeld(Action action) const;       // Key is currently down
    bool justPressed(Action action) const;  // Key was pressed this frame
    bool justReleased(Action action) const; // Key was released this frame

    // For touch/mouse - returns true if any confirm input this frame
    bool anyConfirmInput() const { return confirmInputThisFrame; }

private:
    Input();

    std::unordered_map<SDL_Scancode, Action> keyBindings;
    std::unordered_map<Action, bool> currentState;
    std::unordered_map<Action, bool> previousState;
    bool confirmInputThisFrame = false;

    void bindKey(SDL_Scancode key, Action action);
    void setActionState(Action action, bool pressed);
};

#include "Input.h"

Input::Input() {
    // Default key bindings
    bindKey(SDL_SCANCODE_UP, Action::MoveUp);
    bindKey(SDL_SCANCODE_W, Action::MoveUp);
    bindKey(SDL_SCANCODE_DOWN, Action::MoveDown);
    bindKey(SDL_SCANCODE_S, Action::MoveDown);
    bindKey(SDL_SCANCODE_LEFT, Action::MoveLeft);
    bindKey(SDL_SCANCODE_A, Action::MoveLeft);
    bindKey(SDL_SCANCODE_RIGHT, Action::MoveRight);
    bindKey(SDL_SCANCODE_D, Action::MoveRight);
    bindKey(SDL_SCANCODE_SPACE, Action::Confirm);
    bindKey(SDL_SCANCODE_RETURN, Action::Confirm);
    bindKey(SDL_SCANCODE_ESCAPE, Action::Back);
    bindKey(SDL_SCANCODE_AC_BACK, Action::Back);  // Android back button
    bindKey(SDL_SCANCODE_P, Action::Pause);

    // Jump uses same keys as Confirm + Up arrow
    // (can't bind same key to multiple actions, so we'll check both in game code)

    // Initialize all actions to not pressed
    for (int i = 0; i <= static_cast<int>(Action::Jump); i++) {
        Action action = static_cast<Action>(i);
        currentState[action] = false;
        previousState[action] = false;
    }
}

void Input::bindKey(SDL_Scancode key, Action action) {
    keyBindings[key] = action;
}

void Input::beginFrame() {
    // Copy current state to previous state
    previousState = currentState;
    confirmInputThisFrame = false;

    // Also check keyboard state for held keys (for continuous movement)
    const bool* keys = SDL_GetKeyboardState(NULL);
    for (const auto& [scancode, action] : keyBindings) {
        if (keys[scancode]) {
            currentState[action] = true;
        }
    }
}

void Input::processEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        auto it = keyBindings.find(event.key.scancode);
        if (it != keyBindings.end()) {
            setActionState(it->second, true);
            if (it->second == Action::Confirm) {
                confirmInputThisFrame = true;
                // Jump triggers on same keys as Confirm
                setActionState(Action::Jump, true);
            }
            // Up arrow also triggers Jump
            if (it->second == Action::MoveUp) {
                setActionState(Action::Jump, true);
            }
        }
    }
    else if (event.type == SDL_EVENT_KEY_UP) {
        auto it = keyBindings.find(event.key.scancode);
        if (it != keyBindings.end()) {
            setActionState(it->second, false);
            if (it->second == Action::Confirm || it->second == Action::MoveUp) {
                setActionState(Action::Jump, false);
            }
        }
    }
    else if (event.type == SDL_EVENT_FINGER_DOWN ||
             event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        confirmInputThisFrame = true;
        setActionState(Action::Jump, true);
    }
    else if (event.type == SDL_EVENT_FINGER_UP ||
             event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        setActionState(Action::Jump, false);
    }
}

void Input::setActionState(Action action, bool pressed) {
    currentState[action] = pressed;
}

bool Input::isHeld(Action action) const {
    auto it = currentState.find(action);
    return it != currentState.end() && it->second;
}

bool Input::justPressed(Action action) const {
    auto curr = currentState.find(action);
    auto prev = previousState.find(action);
    bool currPressed = (curr != currentState.end() && curr->second);
    bool prevPressed = (prev != previousState.end() && prev->second);
    return currPressed && !prevPressed;
}

bool Input::justReleased(Action action) const {
    auto curr = currentState.find(action);
    auto prev = previousState.find(action);
    bool currPressed = (curr != currentState.end() && curr->second);
    bool prevPressed = (prev != previousState.end() && prev->second);
    return !currPressed && prevPressed;
}

#include "Input.h"

Input* Input::singleton = nullptr;

Input::Input()
{
    keys = new KeyState[sf::Keyboard::KeyCount];

    for (int k = 0; k < sf::Keyboard::KeyCount;  k++) {
        keys[k] == KeyState::UP;
    }
}

Input* Input::GetSingleton()
{
    if (singleton == nullptr) {
        singleton = new Input();
    }
    return singleton;
}

void Input::UpdateEndOfFrame()
{
    // Reset the vertical scroll to 0
    GetSingleton()->vScrollDelta = 0;

    // Update all keys moving all pressed/released -> down/up
    for (int k = 0; k < sf::Keyboard::KeyCount; k++) {
        if (GetSingleton()->keys[k] == KeyState::PRESSED) GetSingleton()->keys[k] = KeyState::DOWN;
        else if (GetSingleton()->keys[k] == KeyState::RELEASED) GetSingleton()->keys[k] = KeyState::UP;
        else if (GetSingleton()->keys[k] == KeyState::PRESSED_AND_RELEASED_SAME_FRAME) GetSingleton()->keys[k] = KeyState::UP;
    }
}

void Input::SetVScrollDelta(float delta)
{
    GetSingleton()->vScrollDelta = delta;
}

float Input::GetVScrollDelta()
{
    return GetSingleton()->vScrollDelta;
}

void Input::SetMouseX(int value)
{
    GetSingleton()->mouseX = value;
}

void Input::SetMouseY(int value)
{
    GetSingleton()->mouseY = value;
}

int Input::GetMouseX()
{
    return GetSingleton()->mouseX;
}

int Input::GetMouseY()
{
    return GetSingleton()->mouseY;
}

void Input::LogKeyPress(sf::Keyboard::Key key)
{
    GetSingleton()->keys[key] = KeyState::PRESSED;
}

void Input::LogKeyRelease(sf::Keyboard::Key key)
{
    if (GetSingleton()->keys[key] == KeyState::PRESSED) GetSingleton()->keys[key] = KeyState::PRESSED_AND_RELEASED_SAME_FRAME;
    else GetSingleton()->keys[key] = KeyState::RELEASED;
}

bool Input::IsKeyPressed(sf::Keyboard::Key key)
{
    return (GetSingleton()->keys[key] == KeyState::PRESSED || GetSingleton()->keys[key] == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
}

bool Input::IsKeyDown(sf::Keyboard::Key key)
{
    return (GetSingleton()->keys[key] == KeyState::PRESSED || GetSingleton()->keys[key] == KeyState::DOWN || GetSingleton()->keys[key] == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
}

bool Input::IsKeyReleased(sf::Keyboard::Key key)
{
    return (GetSingleton()->keys[key] == KeyState::RELEASED || GetSingleton()->keys[key] == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
}

bool Input::IsKeyUp(sf::Keyboard::Key key)
{
    return (GetSingleton()->keys[key] == KeyState::RELEASED || GetSingleton()->keys[key] == KeyState::UP || GetSingleton()->keys[key] == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
}

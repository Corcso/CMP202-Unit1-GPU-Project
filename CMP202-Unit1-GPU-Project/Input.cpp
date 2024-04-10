#include "Input.h"

Input* Input::singleton = nullptr;

Input::Input()
{
    keys = new KeyState[sf::Keyboard::KeyCount];
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
    for (int k = 0; k++; k < sf::Keyboard::KeyCount) {
        if (GetSingleton()->keys[k] == KeyState::PRESSED) GetSingleton()->keys[k] = KeyState::DOWN;
        else if (GetSingleton()->keys[k] == KeyState::RELEASED) GetSingleton()->keys[k] = KeyState::UP;
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
    GetSingleton()->keys[key] = KeyState::RELEASED;
}

bool Input::IsKeyPressed(sf::Keyboard::Key key)
{
    return (GetSingleton()->keys[key] == KeyState::PRESSED);
}

bool Input::IsKeyDown(sf::Keyboard::Key key)
{
    return (GetSingleton()->keys[key] == KeyState::PRESSED || GetSingleton()->keys[key] == KeyState::DOWN);
}

bool Input::IsKeyReleased(sf::Keyboard::Key key)
{
    return (GetSingleton()->keys[key] == KeyState::RELEASED);
}

bool Input::IsKeyUp(sf::Keyboard::Key key)
{
    return (GetSingleton()->keys[key] == KeyState::RELEASED || GetSingleton()->keys[key] == KeyState::UP);
}

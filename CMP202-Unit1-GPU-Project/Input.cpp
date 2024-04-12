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

    // Update the mouse buttons
    if (GetSingleton()->mouseLeft == KeyState::PRESSED) GetSingleton()->mouseLeft = KeyState::DOWN;
    else if (GetSingleton()->mouseLeft == KeyState::RELEASED) GetSingleton()->mouseLeft = KeyState::UP;
    else if (GetSingleton()->mouseLeft == KeyState::PRESSED_AND_RELEASED_SAME_FRAME) GetSingleton()->mouseLeft = KeyState::UP;

    if (GetSingleton()->mouseRight == KeyState::PRESSED) GetSingleton()->mouseRight = KeyState::DOWN;
    else if (GetSingleton()->mouseRight == KeyState::RELEASED) GetSingleton()->mouseRight = KeyState::UP;
    else if (GetSingleton()->mouseRight == KeyState::PRESSED_AND_RELEASED_SAME_FRAME) GetSingleton()->mouseRight = KeyState::UP;
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

void Input::LogMousePress(bool left)
{
    if (left) GetSingleton()->mouseLeft = KeyState::PRESSED;
    else GetSingleton()->mouseRight = KeyState::PRESSED;
}

void Input::LogMouseRelease(bool left)
{
    if (left) { 
        if(GetSingleton()->mouseLeft == KeyState::PRESSED) GetSingleton()->mouseLeft = KeyState::PRESSED_AND_RELEASED_SAME_FRAME;
        else GetSingleton()->mouseLeft = KeyState::RELEASED; 
    }
    else {
        if (GetSingleton()->mouseRight == KeyState::PRESSED) GetSingleton()->mouseRight = KeyState::PRESSED_AND_RELEASED_SAME_FRAME;
        else GetSingleton()->mouseRight = KeyState::RELEASED;
    }
}

bool Input::IsMousePressed(bool left)
{
    if(left) return (GetSingleton()->mouseLeft == KeyState::PRESSED || GetSingleton()->mouseLeft == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
    else return (GetSingleton()->mouseRight == KeyState::PRESSED || GetSingleton()->mouseRight == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
}

bool Input::IsMouseDown(bool left)
{
    if (left) return (GetSingleton()->mouseLeft == KeyState::PRESSED || GetSingleton()->mouseLeft == KeyState::DOWN || GetSingleton()->mouseLeft == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
    else return (GetSingleton()->mouseRight == KeyState::PRESSED || GetSingleton()->mouseRight == KeyState::DOWN || GetSingleton()->mouseRight == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
}

bool Input::IsMouseReleased(bool left)
{
    if (left) return (GetSingleton()->mouseLeft == KeyState::RELEASED || GetSingleton()->mouseLeft == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
    else return (GetSingleton()->mouseRight == KeyState::RELEASED || GetSingleton()->mouseRight == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
}

bool Input::IsMouseUp(bool left)
{
    if (left) return (GetSingleton()->mouseLeft == KeyState::RELEASED || GetSingleton()->mouseLeft == KeyState::UP || GetSingleton()->mouseLeft == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
    else return (GetSingleton()->mouseRight == KeyState::RELEASED || GetSingleton()->mouseRight == KeyState::UP || GetSingleton()->mouseRight == KeyState::PRESSED_AND_RELEASED_SAME_FRAME);
}

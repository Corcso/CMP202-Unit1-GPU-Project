#include "Input.h"

Input* Input::singleton = nullptr;

Input* Input::GetSingleton()
{
    if (singleton == nullptr) {
        singleton = new Input();
    }
    return singleton;
}

void Input::UpdateEndOfFrame()
{
    GetSingleton()->vScrollDelta = 0;
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

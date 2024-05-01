#pragma once
#include "SFML/Graphics.hpp"

class Input
{
public:
	Input();

	static Input* GetSingleton();

	static void UpdateEndOfFrame();

	static void SetVScrollDelta(float delta);
	static float GetVScrollDelta();
	static void SetMouseX(int value);
	static void SetMouseY(int value);
	static int GetMouseX();
	static int GetMouseY();

	static void LogKeyPress(sf::Keyboard::Key key);
	static void LogKeyRelease(sf::Keyboard::Key key);
	static bool IsKeyPressed(sf::Keyboard::Key key);
	static bool IsKeyDown(sf::Keyboard::Key key);
	static bool IsKeyReleased(sf::Keyboard::Key key);
	static bool IsKeyUp(sf::Keyboard::Key key);

	static void LogMousePress(bool left = true);
	static void LogMouseRelease(bool left = true);
	static bool IsMousePressed(bool left = true);
	static bool IsMouseDown(bool left = true);
	static bool IsMouseReleased(bool left = true);
	static bool IsMouseUp(bool left = true);

private:
	static Input* singleton;

	// Enum for all possible keystates
	// Keys can be pressed and released before a new frame is processed hence the 5th state. 
	enum class KeyState {PRESSED, DOWN, RELEASED, UP, PRESSED_AND_RELEASED_SAME_FRAME};

	// Array of keystates array index for each key enum.
	KeyState* keys;
	// Use keystate for mouse as its the same pattern
	KeyState mouseLeft; 
	KeyState mouseRight; 

	float vScrollDelta = 0; // Scroll wheel vertical scroll delta
	int mouseX; // Mouse X position
	int mouseY; // Mouse Y position
};


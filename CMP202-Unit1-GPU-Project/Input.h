#pragma once
class Input
{
public:
	static Input* GetSingleton();

	static void UpdateEndOfFrame();

	static void SetVScrollDelta(float delta);
	static float GetVScrollDelta();
	static void SetMouseX(int value);
	static void SetMouseY(int value);
	static int GetMouseX();
	static int GetMouseY();

private:
	static Input* singleton;

	float vScrollDelta;
	int mouseX;
	int mouseY;
};


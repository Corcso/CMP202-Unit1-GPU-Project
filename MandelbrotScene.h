#pragma once
#include <stdint.h>
#include "SFML/Graphics.hpp"
class MandelbrotScene
{
public:

	MandelbrotScene();

	void onUpdate(float deltaTime);
	void onRender(sf::RenderWindow& window, float deltaTime);

private:
	bool calculated = false;
	uint32_t* imageBuffer;
};


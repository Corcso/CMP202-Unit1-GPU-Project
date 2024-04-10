#pragma once
#include <stdint.h>
#include "SFML/Graphics.hpp"
class MandelbrotScene
{
public:

	MandelbrotScene();

	void onUpdate(sf::RenderWindow& window, float deltaTime);
	void onRender(sf::RenderWindow& window, float deltaTime);

private:
	bool calculated = false;
	uint32_t* imageBuffer;

	double left = -2, right = 0, top = 1, bottom = -1;

	int currentMaxIterations = 1000;

	// SFML ASSETS
	sf::Font consolas;

	// SFML Objects
	sf::Text zoomLevel_Text;
	sf::Text maxIterations_Text;
	sf::Text zoomWarning_Text;
};


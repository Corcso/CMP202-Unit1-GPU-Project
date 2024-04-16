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
	// A pointer to the image buffer array, this stores all the pixel values. 
	uint32_t* imageBuffer;

	// This stores true if one of the render parameters has changed and a re render is required. 
	bool reRenderRequired;

	double left = -2, right = 0, top = 1, bottom = -1;

	int currentMaxIterations = 1250;

	// SFML ASSETS
	sf::Font consolas;

	// SFML Objects
	sf::Text zoomLevel_Text;
	sf::Text maxIterations_Text;
	sf::Text zoomWarning_Text;
	sf::Text timeTaken_Text;

	// Box draw zooming variables
	int mouseStartX_BoxDraw;
	int mouseStartY_BoxDraw;
	sf::RectangleShape boxDrawBox;
	bool renderDrawBox;
	
};


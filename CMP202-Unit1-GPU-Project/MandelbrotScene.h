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
	enum class GenerationAlgorithm {
		STANDARD, SUBGROUP_AUTOLIMIT
	};
	GenerationAlgorithm currentGenerationAlgorithm;

	// A pointer to the image buffer array, this stores all the pixel values. 
	uint32_t* imageBuffer;

	// This stores true if one of the render parameters has changed and a re render is required. 
	bool reRenderRequired;

	double left = -2, right = 0, top = 1, bottom = -1;

	int currentMaxIterations = 1000;
	int currentMinIterations = 1000;

	// SFML ASSETS
	sf::Font consolas;

	// SFML Objects
	sf::RectangleShape statsOverlay;
	sf::Text zoomLevel_Text;
	sf::Text maxIterations_Text;
	sf::Text minIterations_Text;
	sf::Text zoomWarning_Text;
	sf::Text timeTaken_Text;

	// Box draw zooming variables
	int mouseStartX_BoxDraw;
	int mouseStartY_BoxDraw;
	sf::RectangleShape boxDrawBox;
	bool renderDrawBox;

	// Settings box
	bool settingsOpen;
	sf::RectangleShape settingsOverlay;
	sf::Text generationAlgoTitle_Text;
	sf::Text generationalgoValue_Text;
	enum class Setting { GENERATION_ALGORITHM };
	Setting currentSetting;
};


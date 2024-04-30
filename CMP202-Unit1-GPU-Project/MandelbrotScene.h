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
		STANDARD, SUBGROUP_AUTOLIMIT, STANDARD_BUFFERS
	};
	GenerationAlgorithm currentGenerationAlgorithm;

	// A pointer to the image buffer array, this stores all the pixel values. 
	uint32_t* imageBuffer;

	// This stores true if one of the render parameters has changed and a re render is required. 
	bool reRenderRequired;

	double left = -2, right = 0, top = 1, bottom = -1;

	int currentMaxIterations = 1000;
	int currentMinIterations = 0;

	// SFML ASSETS
	sf::Font consolas;

	// SFML Objects
	sf::RectangleShape statsOverlay;
	sf::Text zoomLevel_Text;
	sf::Text maxIterations_Text;
	sf::Text minIterations_Text;
	sf::Text zoomWarning_Text;
	sf::Text timeTaken_Text;
	sf::Text iterations_Text;

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
	sf::Text resolutionTitle_Text;
	sf::Text resolutionValue_Text;
	sf::Text deviceTitle_Text;
	sf::Text deviceValue_Text;
	enum class Setting { GENERATION_ALGORITHM, RESOLUTION, DEVICE };
	Setting currentSetting = Setting::GENERATION_ALGORITHM;

	// Settings
	enum class Resolution { r1000x1000, r1920x1080, r500x500, r3440x1440};
	Resolution currentResolutionSetting = Resolution::r1000x1000;
	int width;
	int height;
	float aspect; // width / height

	enum class Device {CPU, GPU};
	Device currentDevice = Device::CPU;
};


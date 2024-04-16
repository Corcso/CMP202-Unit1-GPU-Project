#include "MandelbrotScene.h"
#include "MandelbrotGenerator.h"
#include "Input.h"
#include <chrono>

MandelbrotScene::MandelbrotScene()
{

	reRenderRequired = true;
	imageBuffer = new uint32_t[1000 * 1000];

	// Load assets for SFML
	if (!consolas.loadFromFile("./Assets/consola.ttf"))
	{
		// Add code for error and quit
		return;
	}

	zoomLevel_Text.setFont(consolas);
	maxIterations_Text.setFont(consolas);
	zoomWarning_Text.setFont(consolas);
	timeTaken_Text.setFont(consolas);

	zoomLevel_Text.setFillColor(sf::Color::Red);
	maxIterations_Text.setFillColor(sf::Color::Red);
	timeTaken_Text.setFillColor(sf::Color::Red);
	zoomWarning_Text.setFillColor(sf::Color::Yellow);

	zoomLevel_Text.setCharacterSize(24);
	maxIterations_Text.setCharacterSize(24);
	timeTaken_Text.setCharacterSize(24);
	zoomWarning_Text.setCharacterSize(36);

	zoomLevel_Text.setPosition(sf::Vector2f(0, 0));
	maxIterations_Text.setPosition(sf::Vector2f(0, 30));
	timeTaken_Text.setPosition(sf::Vector2f(0, 60));
	zoomWarning_Text.setPosition(sf::Vector2f(0, 950));

	zoomWarning_Text.setString("APPROACHING PRECISION LIMIT");
	zoomWarning_Text.setPosition(sf::Vector2f(500 - (zoomWarning_Text.getLocalBounds().width / 2), 950));

	renderDrawBox = false;
	boxDrawBox.setOutlineColor(sf::Color::White);
	boxDrawBox.setFillColor(sf::Color::Transparent);
	boxDrawBox.setOutlineThickness(1);
}

void MandelbrotScene::onUpdate(sf::RenderWindow& window, float deltaTime)
{
	// Scroll Zooming
	if (Input::GetVScrollDelta() != 0) {
		//std::cout << "X: " << Input::GetMouseX() << " Y: " << Input::GetMouseY() << " D: " << Input::GetVScrollDelta() << "\n";

		float xAlong = (float)Input::GetMouseX() / window.getSize().x;
		float yAlong = (float)Input::GetMouseY() / window.getSize().y;
		float deltaScroll = Input::GetVScrollDelta();
		//std::cout << "pX: " << xAlong << " pY: " << yAlong << "\n";

		double scrollFactor = (right - left) * 0.1;

		left += xAlong * scrollFactor * deltaScroll;
		right -= (1 - xAlong) * scrollFactor * deltaScroll;
		top -= yAlong * scrollFactor * deltaScroll;
		bottom += (1 - yAlong) * scrollFactor * deltaScroll;

		// Fix aspect ratio if needed
		top = bottom + (right - left);

		std::cout << "ASPECT: " << (right - left) / (top - bottom)  << "DIF: " << (right - left) << "\n";

		// Since the zoom has changed, re render the mandelbrot
		reRenderRequired = true;
	}
	// Box Drawing
	if (Input::IsMousePressed(false)) {
		mouseStartX_BoxDraw = Input::GetMouseX();
		mouseStartY_BoxDraw = Input::GetMouseY();
		renderDrawBox = true;
	} // Else if here because ignore same frame clicks
	else if (Input::IsMouseReleased(false)) {

		// Only zoom to the box if the mouse is at a different position in either direction
		// Dont want boxes with 0 as a dimention
		if (mouseStartX_BoxDraw != Input::GetMouseX() && mouseStartY_BoxDraw != Input::GetMouseY()) {
			left += (double)std::min(mouseStartX_BoxDraw, Input::GetMouseX()) / window.getSize().x * (right - left);
			right = left + (double)std::max(mouseStartX_BoxDraw, Input::GetMouseX()) / window.getSize().x * (right - left);
			top += (double)std::min(mouseStartY_BoxDraw, Input::GetMouseY()) / window.getSize().y * (bottom - top);
			bottom = top + (double)std::max(mouseStartY_BoxDraw, Input::GetMouseY()) / window.getSize().y * (bottom - top);

			// Since the zoom has changed, re render the mandelbrot
			reRenderRequired = true;
		}

		renderDrawBox = false;
	}
	// Set the size and position of the box draw display box
	if (Input::IsMouseDown(false)) {
		boxDrawBox.setPosition(mouseStartX_BoxDraw, mouseStartY_BoxDraw);
		boxDrawBox.setSize(sf::Vector2f(Input::GetMouseX() - mouseStartX_BoxDraw, Input::GetMouseY() - mouseStartY_BoxDraw));
	}

	if (Input::IsKeyPressed(sf::Keyboard::Equal)) {
		// Double the current max iterations
		currentMaxIterations *= 2;

		// Since the iterations have changed, re render the mandelbrot
		reRenderRequired = true;
	}
	if (Input::IsKeyPressed(sf::Keyboard::Hyphen)) {
		// Half the current max iterations
		currentMaxIterations /= 2;

		// Since the iterations have changed, re render the mandelbrot
		reRenderRequired = true;
	}



	if (reRenderRequired) {
		sycl::queue q(sycl::gpu_selector{});
		auto start = std::chrono::steady_clock::now();
		MandelbrotGenerator::GenerateBasic(&q, imageBuffer, 1000, 1000, left, right, top ,bottom, currentMaxIterations);
		auto end = std::chrono::steady_clock::now();
		timeTaken_Text.setString("Generation time: " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) + "mcs");
		//std::cout << "Generated MANDELBROT\n";
		reRenderRequired = false;
	}

	// Change the SFML text assets to be updated with this frames data. 
	zoomLevel_Text.setString("Zoom Level: " + std::to_string(1.0 / ((right - left) / 2.0)) + "x");
	maxIterations_Text.setString("Max Iterations: [-] " + std::to_string(currentMaxIterations) + " [+]");
}

void MandelbrotScene::onRender(sf::RenderWindow& window, float deltaTime)
{
	sf::Texture texture;
	texture.create(1000, 1000);

	sf::Sprite sprite(texture); // needed to draw the texture on screen

	// ...

	//for (register int i = 0; i < 1000 * 200 * 4; i += 4) {
	//	pixels[i] = r; // obviously, assign the values you need here to form your color
	//	pixels[i + 1] = g;
	//	pixels[i + 2] = b;
	//	pixels[i + 3] = a;
	//}

	texture.update((sf::Uint8*)imageBuffer);

	// ...

	window.draw(sprite);
	window.draw(zoomLevel_Text);
	window.draw(maxIterations_Text);
	window.draw(timeTaken_Text);
	if (renderDrawBox) window.draw(boxDrawBox);
	if((right-left) < 0.000000000001) window.draw(zoomWarning_Text);
}

#include "MandelbrotScene.h"
#include "MandelbrotGenerator.h"
#include "Input.h"

MandelbrotScene::MandelbrotScene()
{
	imageBuffer = new uint32_t[1000 * 1000];
}

void MandelbrotScene::onUpdate(sf::RenderWindow& window, float deltaTime)
{

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
	}

	if (Input::IsKeyPressed(sf::Keyboard::Equal)) {
		currentMaxIterations *= 2;
		std::cout << "ITER: " << currentMaxIterations << "\n";
	}
	if (Input::IsKeyPressed(sf::Keyboard::Hyphen)) {
		currentMaxIterations /= 2;
		std::cout << "ITER: " << currentMaxIterations << "\n";
	}



	if (!calculated) {
		sycl::queue q/*(sycl::gpu_selector{})*/;
		MandelbrotGenerator::GenerateBasic(&q, imageBuffer, 1000, 1000, left, right, top ,bottom, currentMaxIterations);
		//std::cout << "Generated MANDELBROT\n";
		//calculated = true;
	}

	/*bottom += 0.02 * deltaTime;
	top -= 0.02 * deltaTime;
	left += 0.02 * deltaTime;
	right -= 0.02 * deltaTime;*/
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
}

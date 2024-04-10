#include "MandelbrotScene.h"
#include "MandelbrotGenerator.h"


MandelbrotScene::MandelbrotScene()
{
	imageBuffer = new uint32_t[1000 * 800];
}

void MandelbrotScene::onUpdate(float deltaTime)
{
	if (!calculated) {
		sycl::queue q/*(sycl::gpu_selector{})*/;
		MandelbrotGenerator::GenerateBasic(&q, imageBuffer, 1000, 800, left, right, top ,bottom);
		std::cout << "Generated MANDELBROT\n";
		//calculated = true;
	}

	bottom += 0.02 * deltaTime;
	top -= 0.02 * deltaTime;
	left += 0.02 * deltaTime;
	right -= 0.02 * deltaTime;
}

void MandelbrotScene::onRender(sf::RenderWindow& window, float deltaTime)
{
	sf::Texture texture;
	texture.create(1000, 800);

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

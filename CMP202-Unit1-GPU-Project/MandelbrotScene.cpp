#include "MandelbrotScene.h"
#include "MandelbrotGenerator.h"
#include "Input.h"
#include <chrono>

MandelbrotScene::MandelbrotScene()
{
	currentGenerationAlgorithm = GenerationAlgorithm::STANDARD;

	reRenderRequired = true;
	width = 1000; height = 1000; aspect = 1;
	imageBuffer = new uint32_t[width * height];

	// Load assets for SFML
	if (!consolas.loadFromFile("./Assets/consola.ttf"))
	{
		// Add code for error and quit
		return;
	}

	statsOverlay.setFillColor(sf::Color(40, 40, 40, 127));
	statsOverlay.setPosition(sf::Vector2f(0, 0));
	statsOverlay.setSize(sf::Vector2f(300, 100));

	zoomLevel_Text.setFont(consolas);
	maxIterations_Text.setFont(consolas);
	minIterations_Text.setFont(consolas);
	zoomWarning_Text.setFont(consolas);
	timeTaken_Text.setFont(consolas);
	iterations_Text.setFont(consolas);

	zoomLevel_Text.setFillColor(sf::Color::White);
	maxIterations_Text.setFillColor(sf::Color::White);
	minIterations_Text.setFillColor(sf::Color::White);
	timeTaken_Text.setFillColor(sf::Color::White);
	iterations_Text.setFillColor(sf::Color::White);
	zoomWarning_Text.setFillColor(sf::Color::Yellow);

	zoomLevel_Text.setCharacterSize(20);
	maxIterations_Text.setCharacterSize(20);
	minIterations_Text.setCharacterSize(20);
	timeTaken_Text.setCharacterSize(20);
	iterations_Text.setCharacterSize(20);
	zoomWarning_Text.setCharacterSize(36);

	zoomLevel_Text.setPosition(sf::Vector2f(0, 0));
	maxIterations_Text.setPosition(sf::Vector2f(0, 25));
	minIterations_Text.setPosition(sf::Vector2f(0, 50));
	timeTaken_Text.setPosition(sf::Vector2f(0, 75));
	iterations_Text.setPosition(sf::Vector2f(0, 100));
	zoomWarning_Text.setPosition(sf::Vector2f(0, 950));

	zoomWarning_Text.setString("APPROACHING PRECISION LIMIT");
	zoomWarning_Text.setPosition(sf::Vector2f(500 - (zoomWarning_Text.getLocalBounds().width / 2), 950));

	renderDrawBox = false;
	boxDrawBox.setOutlineColor(sf::Color::White);
	boxDrawBox.setFillColor(sf::Color::Transparent);
	boxDrawBox.setOutlineThickness(1);

	// Settings menu setup
	currentSetting = Setting::GENERATION_ALGORITHM;
	settingsOpen = false;

	settingsOverlay.setOutlineColor(sf::Color::White);
	settingsOverlay.setFillColor(sf::Color(40, 40, 40, 127));
	settingsOverlay.setOutlineThickness(1);
	settingsOverlay.setPosition(sf::Vector2f(100, 100));
	settingsOverlay.setSize(sf::Vector2f(800, 800));

	generationAlgoTitle_Text.setFont(consolas);
	generationAlgoTitle_Text.setCharacterSize(24);
	generationAlgoTitle_Text.setFillColor(sf::Color::Yellow);
	generationAlgoTitle_Text.setPosition(sf::Vector2f(120, 110));
	generationAlgoTitle_Text.setString("Generation Algorithm");

	generationalgoValue_Text.setFont(consolas);
	generationalgoValue_Text.setCharacterSize(24);
	generationalgoValue_Text.setFillColor(sf::Color::Yellow);
	generationalgoValue_Text.setPosition(sf::Vector2f(120, 135));
	generationalgoValue_Text.setString("Standard");

	resolutionTitle_Text.setFont(consolas);
	resolutionTitle_Text.setCharacterSize(24);
	resolutionTitle_Text.setFillColor(sf::Color::White);
	resolutionTitle_Text.setPosition(sf::Vector2f(120, 175));
	resolutionTitle_Text.setString("Resolution");

	resolutionValue_Text.setFont(consolas);
	resolutionValue_Text.setCharacterSize(24);
	resolutionValue_Text.setFillColor(sf::Color::White);
	resolutionValue_Text.setPosition(sf::Vector2f(120, 200));
	resolutionValue_Text.setString("1000 x 1000");

	deviceTitle_Text.setFont(consolas);
	deviceTitle_Text.setCharacterSize(24);
	deviceTitle_Text.setFillColor(sf::Color::White);
	deviceTitle_Text.setPosition(sf::Vector2f(120, 240));
	deviceTitle_Text.setString("Device");
	
	deviceValue_Text.setFont(consolas);
	deviceValue_Text.setCharacterSize(24);
	deviceValue_Text.setFillColor(sf::Color::White);
	deviceValue_Text.setPosition(sf::Vector2f(120, 265));
	sycl::queue* testQ;
	try {
		testQ = new sycl::queue(sycl::cpu_selector{});
		deviceValue_Text.setString("CPU Found - " + testQ->get_device().get_info<sycl::info::device::name>());
	}
	catch (const sycl::exception& e) {
		testQ = new sycl::queue;
		deviceValue_Text.setString("CPU Not Found - " + testQ->get_device().get_info<sycl::info::device::name>());
	}
	delete testQ;

}

void MandelbrotScene::onUpdate(sf::RenderWindow& window, float deltaTime)
{
	if (Input::IsKeyPressed(sf::Keyboard::Escape)) {
		settingsOpen = !settingsOpen;
		// If exiting the settings then re render to see the new settings right away
		if (!settingsOpen) reRenderRequired = true;
	}
	if (!settingsOpen) {
		// Scroll Zooming
		if (Input::GetVScrollDelta() != 0) {
			//std::cout << "X: " << Input::GetMouseX() << " Y: " << Input::GetMouseY() << " D: " << Input::GetVScrollDelta() << "\n";

			float xAlong = (float)Input::GetMouseX() / window.getSize().x;
			float yAlong = (float)Input::GetMouseY() / window.getSize().y;
			float deltaScroll = std::min(Input::GetVScrollDelta(), 5.0f); // Limit scroll delta to prevent over scroll on laggy frames
			//std::cout << "pX: " << xAlong << " pY: " << yAlong << "\n";

			double scrollFactor = (right - left) * 0.1;

			left += xAlong * scrollFactor * deltaScroll * aspect;
			right -= (1 - xAlong) * scrollFactor * deltaScroll * aspect;
			top -= yAlong * scrollFactor * deltaScroll;
			bottom += (1 - yAlong) * scrollFactor * deltaScroll;

			// Fix aspect ratio if needed
			top = bottom + ((right - left) / aspect);

			//std::cout << "ASPECT: " << (right - left) / (top - bottom) << "DIF: " << (right - left) << "\n";

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
		if (Input::IsKeyPressed(sf::Keyboard::Hyphen) && currentMaxIterations / 2 >= currentMinIterations) {
			// Half the current max iterations
			currentMaxIterations /= 2;

			// Since the iterations have changed, re render the mandelbrot
			reRenderRequired = true;
		}
		if (Input::IsKeyPressed(sf::Keyboard::Num0) && currentMinIterations * 2 <= currentMaxIterations) {
			// Double the current max iterations
			currentMinIterations *= 2;

			// Since the iterations have changed, re render the mandelbrot
			reRenderRequired = true;
		}
		if (Input::IsKeyPressed(sf::Keyboard::Num9)) {
			// Half the current max iterations
			currentMinIterations /= 2;

			// Since the iterations have changed, re render the mandelbrot
			reRenderRequired = true;
		}
		// If R is pressed regenerate the set no matter if anything has changed. 
		if (Input::IsKeyPressed(sf::Keyboard::R)) {
			reRenderRequired = true;
		}



		if (reRenderRequired) {

			sycl::queue* q/*(sycl::gpu_selector{})*/;
			switch (currentDevice) {
			case Device::CPU:
				try {
					q = new sycl::queue(sycl::cpu_selector{});
				}
				catch (const sycl::exception& e) {
					q = new sycl::queue;
				}
				break;
			case Device::GPU:
				try {
					q = new sycl::queue(sycl::gpu_selector{});
				}
				catch (const sycl::exception& e) {
					q = new sycl::queue;
				}
				break;
			}
			int iterationsTaken;
			auto start = std::chrono::steady_clock::now();
			switch (currentGenerationAlgorithm) {
			case GenerationAlgorithm::STANDARD:
				iterationsTaken = MandelbrotGenerator::GenerateBasic(q, imageBuffer, width, height, left, right, top, bottom, currentMaxIterations);
				break;
			case GenerationAlgorithm::SUBGROUP_AUTOLIMIT:
				iterationsTaken = MandelbrotGenerator::GenerateSubgroupAutoprecision(q, imageBuffer, width, height, left, right, top, bottom, currentMinIterations, currentMaxIterations);
				break;
			case GenerationAlgorithm::STANDARD_BUFFERS:
				iterationsTaken = MandelbrotGenerator::GenerateBasicWithBuffers(q, imageBuffer, width, height, left, right, top, bottom, currentMaxIterations);
				break;
			}
			//std::cout << "Iterations: " << MandelbrotGenerator::GenerateBasic(&q, imageBuffer, 1000, 1000, left, right, top ,bottom, currentMaxIterations) << "\n";
			//std::cout << "Iterations: " << MandelbrotGenerator::GenerateSubgroupAutoprecision(&q, imageBuffer, 1000, 1000, left, right, top, bottom, currentMinIterations, currentMaxIterations) << "\n";
			auto end = std::chrono::steady_clock::now();
			timeTaken_Text.setString("Generation time: " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) + "mcs");
			iterations_Text.setString("Iterations: " + std::to_string(iterationsTaken));
			//std::cout << "Generated MANDELBROT\n";
			reRenderRequired = false;

			// Delete the queue
			delete q;
		}

		// Change the SFML text assets to be updated with this frames data. 
		zoomLevel_Text.setString("Zoom Level: " + std::to_string(1.0 / ((right - left) / 2.0)) + "x");
		maxIterations_Text.setString("Max Iterations: [-] " + std::to_string(currentMaxIterations) + " [+]");
		minIterations_Text.setString("Min Iterations: [-] " + std::to_string(currentMinIterations) + " [+]");

	}
	// ==========================
	// SETTINGS MENU UPDATE
	// ==========================
	else {
		// First handle input of changing the setting we are editing
		if (Input::IsKeyPressed(sf::Keyboard::Down)) {
			// Increase the enum by 1
			currentSetting = (Setting)((((int)currentSetting) + 1) % 3);
		}
		if (Input::IsKeyPressed(sf::Keyboard::Up)) {
			// Increase the enum by total possibilities - 1 moving it back 1
			currentSetting = (Setting)((((int)currentSetting) + 2) % 3);
		}
		switch (currentSetting) {
		case Setting::GENERATION_ALGORITHM:

			generationAlgoTitle_Text.setFillColor(sf::Color::Yellow);
			generationalgoValue_Text.setFillColor(sf::Color::Yellow);
			resolutionTitle_Text.setFillColor(sf::Color::White);
			resolutionValue_Text.setFillColor(sf::Color::White);
			deviceTitle_Text.setFillColor(sf::Color::White);
			deviceValue_Text.setFillColor(sf::Color::White);

			break;
		case Setting::RESOLUTION:

			generationAlgoTitle_Text.setFillColor(sf::Color::White);
			generationalgoValue_Text.setFillColor(sf::Color::White);
			resolutionTitle_Text.setFillColor(sf::Color::Yellow);
			resolutionValue_Text.setFillColor(sf::Color::Yellow);
			deviceTitle_Text.setFillColor(sf::Color::White); 
			deviceValue_Text.setFillColor(sf::Color::White);

			break;
		case Setting::DEVICE:

			generationAlgoTitle_Text.setFillColor(sf::Color::White);
			generationalgoValue_Text.setFillColor(sf::Color::White);
			resolutionTitle_Text.setFillColor(sf::Color::White);
			resolutionValue_Text.setFillColor(sf::Color::White);
			deviceTitle_Text.setFillColor(sf::Color::Yellow);
			deviceValue_Text.setFillColor(sf::Color::Yellow);

			break;
		}
		// Next handle input of changing the setting itself
		if (Input::IsKeyPressed(sf::Keyboard::Right)) {
			// Create test Q, used when switching devices;
			sycl::queue* testQ;
			switch (currentSetting) {
			case Setting::GENERATION_ALGORITHM:

				// Increase the enum by 1
				currentGenerationAlgorithm = (GenerationAlgorithm)((((int)currentGenerationAlgorithm) + 1) % 3);

				switch (currentGenerationAlgorithm) {
				case GenerationAlgorithm::STANDARD:
					generationalgoValue_Text.setString("Standard");
					currentMinIterations = 0;
					break;
				case GenerationAlgorithm::SUBGROUP_AUTOLIMIT:
					generationalgoValue_Text.setString("Subgroup AutoLimit");
					currentMinIterations = currentMaxIterations;
					break;
				case GenerationAlgorithm::STANDARD_BUFFERS:
					generationalgoValue_Text.setString("Standard with Buffers");
					currentMinIterations = currentMaxIterations;
					break;
				}

				break;
			case Setting::RESOLUTION:
				// Increase the enum by 1
				currentResolutionSetting = (Resolution)((((int)currentResolutionSetting) + 1) % 4);
				delete imageBuffer;
				switch (currentResolutionSetting) {
				case Resolution::r1000x1000:
					resolutionValue_Text.setString("1000 x 1000");
					width = 1000;
					height = 1000; 
					aspect = 1;
					break;
				case Resolution::r1920x1080:
					resolutionValue_Text.setString("1920 x 1080");
					width = 1920;
					height = 1080;
					aspect = (float)1920 / 1080;
					break;
				case Resolution::r500x500:
					resolutionValue_Text.setString("500 x 500");
					width = 500;
					height = 500;
					aspect = 1;
					break;
				case Resolution::r3440x1440:
					resolutionValue_Text.setString("3440 x 1440");
					width = 3440;
					height = 1440;
					aspect = (float)3440 / 1440;
					break;
				}
				imageBuffer = new uint32_t[width * height];
				window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
				break;
			case Setting::DEVICE:
				// Increase the enum by 1
				currentDevice = (Device)((((int)currentDevice) + 1) % 2);
				switch (currentDevice) {
				case Device::CPU:
					deviceValue_Text.setString("CPU");

					try {
						testQ = new sycl::queue(sycl::cpu_selector{});
						deviceValue_Text.setString("CPU Found - " + testQ->get_device().get_info<sycl::info::device::name>());
					}
					catch (const sycl::exception& e) {
						testQ = new sycl::queue;
						deviceValue_Text.setString("CPU Not Found - " + testQ->get_device().get_info<sycl::info::device::name>());
					}

					break;
				case Device::GPU:
					deviceValue_Text.setString("GPU");

					try {
						testQ = new sycl::queue(sycl::gpu_selector{});
						deviceValue_Text.setString("GPU Found - " + testQ->get_device().get_info<sycl::info::device::name>());
					}
					catch (const sycl::exception& e) {
						testQ = new sycl::queue;
						deviceValue_Text.setString("GPU Not Found - " + testQ->get_device().get_info<sycl::info::device::name>());
					}

					break;
				}
				break;
			}

			// Delete the test queue
			delete testQ;
		}
		if (Input::IsKeyPressed(sf::Keyboard::Left)) {
			// Create test Q, used when switching devices;
			sycl::queue* testQ;
			switch (currentSetting) {
			case Setting::GENERATION_ALGORITHM:

				// Increase the enum by total possibilities - 1 moving it back 1
				currentGenerationAlgorithm = (GenerationAlgorithm)((((int)currentGenerationAlgorithm) + 2) % 3);

				switch (currentGenerationAlgorithm) {
				case GenerationAlgorithm::STANDARD:
					generationalgoValue_Text.setString("Standard");
					currentMinIterations = 0;
					break;
				case GenerationAlgorithm::SUBGROUP_AUTOLIMIT:
					generationalgoValue_Text.setString("Subgroup AutoLimit");
					currentMinIterations = currentMaxIterations;
					break;
				case GenerationAlgorithm::STANDARD_BUFFERS:
					generationalgoValue_Text.setString("Standard with Buffers");
					currentMinIterations = currentMaxIterations;
					break;
				}

				break;
			case Setting::RESOLUTION:
				// Decrease the enum by 1 (By increasing by total - 1)
				currentResolutionSetting = (Resolution)((((int)currentResolutionSetting) + 3) % 4);
				delete imageBuffer;
				switch (currentResolutionSetting) {
				case Resolution::r1000x1000:
					resolutionValue_Text.setString("1000 x 1000");
					width = 1000;
					height = 1000;
					aspect = 1;
					break;
				case Resolution::r1920x1080:
					resolutionValue_Text.setString("1920 x 1080");
					width = 1920;
					height = 1080;
					aspect = (float)1920 / 1080;
					break;
				case Resolution::r500x500:
					resolutionValue_Text.setString("500 x 500");
					width = 500;
					height = 500;
					aspect = 1;
					break;
				case Resolution::r3440x1440:
					resolutionValue_Text.setString("3440 x 1440");
					width = 3440;
					height = 1440;
					aspect = (float)3440 / 1440;
					break;
				}
				imageBuffer = new uint32_t[width * height];
				window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
				break;
			case Setting::DEVICE:
				// Decrease the enum by 1 (By increasing by total - 1)
				currentDevice = (Device)((((int)currentDevice) + 1) % 2);
				switch (currentDevice) {
				case Device::CPU:
					deviceValue_Text.setString("CPU");
					
					try {
						testQ = new sycl::queue(sycl::cpu_selector{});
						deviceValue_Text.setString("CPU Found - " + testQ->get_device().get_info<sycl::info::device::name>());
					}
					catch (const sycl::exception& e) {
						testQ = new sycl::queue;
						deviceValue_Text.setString("CPU Not Found - " + testQ->get_device().get_info<sycl::info::device::name>());
					}
					
					break;
				case Device::GPU:
					deviceValue_Text.setString("GPU");

					try {
						testQ = new sycl::queue(sycl::gpu_selector{});
						deviceValue_Text.setString("GPU Found - " + testQ->get_device().get_info<sycl::info::device::name>());
					}
					catch (const sycl::exception& e) {
						testQ = new sycl::queue;
						deviceValue_Text.setString("GPU Not Found - " + testQ->get_device().get_info<sycl::info::device::name>());
					}

					break;
				}
				break;
			}

			// Delete the test queue
			delete testQ;
		}
	}
	// ===============
	// MISC GUI UPDATE
	// ===============
	statsOverlay.setSize(sf::Vector2f(std::max(std::max(
		std::max(zoomLevel_Text.getGlobalBounds().width,
			maxIterations_Text.getGlobalBounds().width),
		std::max(minIterations_Text.getGlobalBounds().width,
			timeTaken_Text.getGlobalBounds().width)),
		iterations_Text.getGlobalBounds().width
	) + 10, 130));
}

void MandelbrotScene::onRender(sf::RenderWindow& window, float deltaTime)
{
	sf::Texture texture;
	texture.create(width, height);

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
	window.draw(statsOverlay);
	window.draw(zoomLevel_Text);
	window.draw(maxIterations_Text);
	if(currentGenerationAlgorithm == GenerationAlgorithm::SUBGROUP_AUTOLIMIT) window.draw(minIterations_Text);
	window.draw(timeTaken_Text);
	window.draw(iterations_Text);
	if (renderDrawBox) window.draw(boxDrawBox);
	if((right-left) < 0.000000000001) window.draw(zoomWarning_Text);
	if (settingsOpen) {
		window.draw(settingsOverlay);
		window.draw(generationAlgoTitle_Text);
		window.draw(generationalgoValue_Text);
		window.draw(resolutionTitle_Text);
		window.draw(resolutionValue_Text);
		window.draw(deviceTitle_Text);
		window.draw(deviceValue_Text);
	}
}

#include <SFML/Graphics.hpp>

#include "MandelbrotScene.h"
#include "Input.h"

int main()
{
    // Scene object where all update and render code goes. 
    MandelbrotScene mainScene;

    // Create the window object in SFML
    sf::RenderWindow window(sf::VideoMode(1000, 800), "CMP 202 GPU");

    // Start the clock for getting deltatime
    sf::Clock clock;
    while (window.isOpen())
    {
        // Get the delta time
        float deltaTime = clock.restart().asSeconds();

        // Handle a window close
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    Input::SetVScrollDelta(event.mouseWheelScroll.delta);
                    Input::SetMouseX(event.mouseWheelScroll.x);
                    Input::SetMouseY(event.mouseWheelScroll.y);
                }
            }
        }

        // Every frame run the scenes update and render functions. 
        mainScene.onUpdate(deltaTime);
        
        window.clear();
        mainScene.onRender(window, deltaTime);
        window.display();
    }

    return 0;
}
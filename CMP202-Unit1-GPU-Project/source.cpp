#include <SFML/Graphics.hpp>
#include <iostream>
#include "MandelbrotScene.h"
#include "Input.h"

int main()
{
    // Scene object where all update and render code goes. 
    MandelbrotScene mainScene;

    // Create the window object in SFML
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "CMP 202 GPU");
    window.setKeyRepeatEnabled(false);
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
            else if (event.type == sf::Event::KeyPressed) {
                Input::LogKeyPress(event.key.code);
                std::cout << event.key.code << "\n";
            }
            else if (event.type == sf::Event::KeyReleased) {
                Input::LogKeyRelease(event.key.code);
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right) Input::LogMousePress(false);
                else if (event.mouseButton.button == sf::Mouse::Left) Input::LogMousePress();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Right) Input::LogMouseRelease(false);
                else if (event.mouseButton.button == sf::Mouse::Left) Input::LogMouseRelease();
            }
        }

        // Every frame run the scenes update and render functions. 
        mainScene.onUpdate(window, deltaTime);
        
        window.clear();
        mainScene.onRender(window, deltaTime);
        window.display();

        Input::UpdateEndOfFrame();
    }

    return 0;
}
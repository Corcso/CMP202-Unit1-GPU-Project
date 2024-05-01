#include <SFML/Graphics.hpp>
#include <iostream>
#include "MandelbrotScene.h"
#include "Input.h"

int main()
{
    // Create the window object in SFML
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "CMP 202 GPU");
    window.setKeyRepeatEnabled(false);

    // Scene object where all update and render code goes. 
    MandelbrotScene mainScene;

    // Start the clock for getting deltatime
    sf::Clock clock;
    while (window.isOpen())
    {
        // Get the delta time
        float deltaTime = clock.restart().asSeconds();

        // Handle all events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed
            if (event.type == sf::Event::Closed)
                window.close();
            // Mouse scrolled
            else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    Input::SetVScrollDelta(event.mouseWheelScroll.delta);
                    Input::SetMouseX(event.mouseWheelScroll.x);
                    Input::SetMouseY(event.mouseWheelScroll.y);
                }
            }
            // Key Pressed and Released
            else if (event.type == sf::Event::KeyPressed) {
                Input::LogKeyPress(event.key.code);
                std::cout << event.key.code << "\n";
            }
            else if (event.type == sf::Event::KeyReleased) {
                Input::LogKeyRelease(event.key.code);
            }
            // Mouse Pressed and Released
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right) Input::LogMousePress(false);
                else if (event.mouseButton.button == sf::Mouse::Left) Input::LogMousePress();
                Input::SetMouseX(event.mouseButton.x);
                Input::SetMouseY(event.mouseButton.y);
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Right) Input::LogMouseRelease(false);
                else if (event.mouseButton.button == sf::Mouse::Left) Input::LogMouseRelease();
                Input::SetMouseX(event.mouseButton.x);
                Input::SetMouseY(event.mouseButton.y);
            }
            // Mouse moved
            else if (event.type == sf::Event::MouseMoved) {
                Input::SetMouseX(event.mouseMove.x);
                Input::SetMouseY(event.mouseMove.y);
            }
        }

        // Every frame run the scenes update and render functions. 
        mainScene.onUpdate(window, deltaTime);
        
        window.clear();
        mainScene.onRender(window, deltaTime);
        window.display();

        // Run input updates for end of frame
        // Things like Pressed -> Down etc. 
        Input::UpdateEndOfFrame();
    }

    return 0;
}
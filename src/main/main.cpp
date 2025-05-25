#include <print>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
// Include the necessary SFML headers

int main () {
    
    // Print the version of SFML
    std::print("SFML Version: {0}.{1}.{2}\n", SFML_VERSION_MAJOR, SFML_VERSION_MINOR, SFML_VERSION_PATCH);

    // Create a window with SFML
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Window");
    // Set the frame rate limit
    window.setFramerateLimit(60);
    // Main loop - runs until the window is closed
    while (window.isOpen()) {
        // Event processing
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>() || 
            (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            window.close();
        }

        // Clear the whole window before rendering a new frame
        window.clear(sf::Color::Black);

        // Draw some graphical entities (for example, a circle)
        sf::CircleShape circle(50);
        circle.setFillColor(sf::Color::Green);
        circle.setPosition({375.0f, 275.0f}); // Center the circle
        window.draw(circle);

        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;
}
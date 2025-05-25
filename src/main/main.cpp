#include <print>

// Include the necessary SFML headers
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main () {
    
    // Print the version of SFML
    std::print("SFML Version: {0}.{1}.{2}\n", SFML_VERSION_MAJOR, SFML_VERSION_MINOR, SFML_VERSION_PATCH);

    // Define constants
    constexpr sf::Vector2u windowSize{800, 600};
    constexpr sf::ContextSettings contextSettings{24, 8, 4, 3, 3}; // 24 bits color depth, 8 bits stencil buffer, 4 samples for anti-aliasing, OpenGL version 3.3
    constexpr auto windowStyle = sf::Style::Default; // Default window style
    constexpr auto windowState = sf::State::Windowed; // Windowed state
    constexpr sf::Vector2i playerSize = {64, 64};
    constexpr sf::Vector2f playerStart({windowSize.x / 2.0f - playerSize.x / 2.0f, static_cast<float>(windowSize.y) - playerSize.y});

    // Create a window with SFML
    sf::RenderWindow window(sf::VideoMode(windowSize), "SFML Window", windowStyle, windowState, contextSettings);
    // Set the frame rate limit
    window.setFramerateLimit(60);

    // Load assets
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../assets/textures/bg1.png")) {
        std::print("Failed to load background texture\n");
        return -1; // Exit if the texture fails to load
    }
    backgroundTexture.setSmooth(true);
    sf::Texture spriteSheetTexture;
    if (!spriteSheetTexture.loadFromFile("../assets/textures/sprites.png")) {
        std::print("Failed to load sprite sheet texture\n");
        return -1; // Exit if the texture fails to load
    }
    spriteSheetTexture.setSmooth(true); // Enable smooth scaling for the sprite sheet texture
    
    // Create sprites to display the textures
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::Sprite playerSprite(spriteSheetTexture);
    playerSprite.setTextureRect(sf::IntRect({0, 0}, playerSize));
    playerSprite.setPosition(playerStart);

    // Event handlers
    const auto onClose = [&window](const sf::Event::Closed&) {
            window.close();
    };
    const auto keyPressed = [&window, &playerSprite](const sf::Event::KeyPressed& keyEvent) {
        if (keyEvent.code == sf::Keyboard::Key::Escape) {
            window.close();
        }

        if (keyEvent.code == sf::Keyboard::Key::Left) {
            playerSprite.move({-10, 0}); // Move left
        } else if (keyEvent.code == sf::Keyboard::Key::Right) {
            playerSprite.move({10, 0}); // Move right
        } else if (keyEvent.code == sf::Keyboard::Key::Up) {
            playerSprite.move({0, -10}); // Move up
        } else if (keyEvent.code == sf::Keyboard::Key::Down) {
            playerSprite.move({0, 10}); // Move down
        }
        else if (keyEvent.code == sf::Keyboard::Key::Space) {
            // Example action for space key, e.g., jump or shoot
            std::print("Space key pressed\n");
        }
    };

    // Main loop - runs until the window is closed
    while (window.isOpen()) {
        window.handleEvents(onClose, keyPressed);

        window.clear(sf::Color::Black);
        
        window.draw(backgroundSprite);
        window.draw(playerSprite);

        window.display();
    }

    return 0;
}
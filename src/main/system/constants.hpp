#pragma once

namespace OneGunGame {
    constexpr sf::Vector2u DefaultWindowSize{800, 600};
    constexpr sf::ContextSettings DefaultContextSettings{24, 8, 4, 3, 3}; // 24 bits color depth, 8 bits stencil buffer, 4 samples for anti-aliasing, OpenGL version 3.3
    constexpr auto DefaultWindowStyle = sf::Style::Default;
    constexpr auto DefaultWindowState = sf::State::Fullscreen;
    constexpr auto DefaultFrameRateLimit = 60;
    constexpr auto WindowTitle = "One Gun - SFML";

    constexpr auto BackgroundPath = "./assets/textures/bg1seamless.png";
    constexpr auto SpriteSheetPath = "./assets/textures/sprites.png";
    
    constexpr auto Pi = 3.1415926535f;
    constexpr auto PiSquared = Pi * Pi;
    constexpr auto HalfPi = Pi / 2.0f;

    enum CollisionLayer {
        Player = 1 << 0,      // Player layer
        Projectile = 1 << 1,  // Projectile layer
        Enemy = 1 << 2,       // Enemy layer
        Obstacle = 1 << 3     // Obstacle layer
    };
}
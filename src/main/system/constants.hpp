#pragma once

namespace OneGunGame {
    struct {
        constexpr static auto WindowSize = sf::Vector2u{800, 600};
        constexpr static auto ContextSettings = sf::ContextSettings{24, 8, 4, 3, 3}; // 24 bits color depth, 8 bits stencil buffer, 4 samples for anti-aliasing, OpenGL version 3.3
        constexpr static auto WindowState = sf::State::Windowed;
        constexpr static auto FrameRateLimit = 60;
    } Default;
    
    struct {
        sf::Vector2u WindowSize = Default.WindowSize;
        sf::ContextSettings ContextSettings = Default.ContextSettings;
        sf::State WindowState = Default.WindowState;
        int FrameRateLimit = Default.FrameRateLimit;
    } Configuration;
    
    constexpr auto WindowTitle = "One Gun - SFML";
    constexpr auto WindowStyle = sf::Style::Titlebar;

    constexpr auto Pi = 3.1415926535f;
    constexpr auto PiSquared = Pi * Pi;
    constexpr auto HalfPi = Pi / 2.0f;

    enum CollisionLayer {
        Player = 1 << 0,      // Player layer
        Projectile = 1 << 1,  // Projectile layer
        Enemy = 1 << 2,       // Enemy layer
        Obstacle = 1 << 3     // Obstacle layer
    };

    enum Images {
        Background = 0,
        SpriteSheet,
        ExplosionRed,
        ExplosionYellow,
        ExplosionGreen,
        ExplosionBlue,
        ExplosionViolet,
        Unknown,
        TotalImages
    };
}
#pragma once

namespace OneGunGame {
using Callback = std::function<void(entt::registry& registry, entt::entity thisEntity)>;

struct Default {
    static constexpr sf::Vector2u WindowSize = sf::Vector2u{1600, 1200};
    static constexpr sf::ContextSettings ContextSettings = sf::ContextSettings{24, 8, 4, 3, 3}; // 24 bits color depth, 8 bits stencil buffer, 4 samples for anti-aliasing, OpenGL version 3.3
    static constexpr sf::State WindowState = sf::State::Windowed;
    static constexpr uint16_t FrameRateLimit = 60;
};

struct {
    //sf::Vector2u WindowSize = Default::WindowSize; // For if I want to use preset window sizes
    sf::ContextSettings ContextSettings = Default::ContextSettings;
    sf::State WindowState = Default::WindowState;
    int FrameRateLimit = Default::FrameRateLimit;
} Configuration;

constexpr auto WindowTitle = "One Gun - SFML";
constexpr auto WindowStyle = sf::Style::Default;
constexpr auto ViewSize = sf::Vector2f{800.0f, 600.0f};

constexpr auto Pi = 3.1415926535f;
constexpr auto PiSquared = Pi * Pi;
constexpr auto HalfPi = Pi / 2.0f;

enum class CollisionLayer : uint8_t {
    Player = 1 << 0,
    PlayerProjectile = 1 << 1,
    Enemy = 1 << 2,
    EnemyProjectile = 1 << 3,
    Obstacle = 1 << 4,
    NeutralProjectile = 1 << 5
};

constexpr uint8_t GetCollisionMask(std::initializer_list<CollisionLayer> layers) {
    uint8_t mask = 0;
    for (CollisionLayer layer : layers) {
        mask &= static_cast<uint8_t>(layer);
    }
    return mask;
}

enum class Images : uint8_t {
    BG = 0,
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
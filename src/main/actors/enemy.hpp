#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace Enemy {
    enum Type {
        Asteroid = 0,
        LargeAsteroid,
        HugeAsteroid,
        Comet,
        Drone,
        Fighter,
        Bomber,
        Hunter,
        Bombardier,
        Galaxis,
        Total
    };

    entt::entity Create(entt::registry &registry, 
        Type type, const sf::Texture &texture, 
        const sf::Vector2f& position, const sf::Vector2f& direction);

    struct Data {
        sf::IntRect TextureRect;
        sf::IntRect CollisionRect;
        float MoveSpeed;
        float OffscreenLifetime;
    }; 
    
    constexpr std::array<Data, Type::Total> Presets = {
        Data{ sf::IntRect({64, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 10.0f, 1.0f},  // Asteroid
        Data{ sf::IntRect({128, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 10.0f, 1.0f}, // LargeAsteroid
        Data{ sf::IntRect({192, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 10.0f, 1.0f}, // HugeAsteroid
        Data{ sf::IntRect({256, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f},  // Comet
        Data{ sf::IntRect({320, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f},  // Drone
        Data{ sf::IntRect({384, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f},  // Fighter
        Data{ sf::IntRect({448, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 4.0f, 1.5f},  // Bomber
        Data{ sf::IntRect({576, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 3.0f},  // Hunter
        Data{ sf::IntRect({0, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 5.0f, 6.0f},   // Bombardier
        Data{ sf::IntRect({64, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 2.5f, 2.0f}   // Galaxis
    };
}
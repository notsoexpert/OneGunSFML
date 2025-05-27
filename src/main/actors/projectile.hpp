#pragma once

#include <SFML/Graphics/Sprite.hpp>

namespace Projectile {
    enum Type {
        Bullet1 = 0,
        Bullet2,
        Bullet3,
        Laser1,
        Laser2,
        Laser3,
        Plasma,
        Missile,
        HomingMissile,
        Bomb,
        Total
    };

    struct Data {
        sf::IntRect TextureRect;
        float Speed;
        float Lifetime;
        bool IsHoming;
    }; 
    static constexpr std::array<Data, Type::Total> Presets = {
        Data{ sf::IntRect({64, 0}, {64, 64}), 10.0f, 5.0f, false}, // Bullet1
        Data{ sf::IntRect({128, 0}, {64, 64}), 10.0f, 5.0f, false}, // Bullet2
        Data{ sf::IntRect({192, 0}, {64, 64}), 10.0f, 5.0f, false}, // Bullet3
        Data{ sf::IntRect({256, 0}, {64, 64}), 6.0f, 5.0f, false}, // Laser1
        Data{ sf::IntRect({320, 0}, {64, 64}), 6.0f, 5.0f, false}, // Laser2
        Data{ sf::IntRect({384, 0}, {64, 64}), 6.0f, 5.0f, false}, // Laser3
        Data{ sf::IntRect({448, 0}, {64, 64}), 4.0f, 8.0f, false}, // Plasma
        Data{ sf::IntRect({576, 0}, {64, 64}), 6.0f, 8.0f, false}, // Missile
        Data{ sf::IntRect({0, 64}, {64, 64}), 5.0f, 12.0f, true}, // HomingMissile
        Data{ sf::IntRect({64, 64}, {64, 64}), 2.5f, 2.0f, false}  // Bomb
    };
}
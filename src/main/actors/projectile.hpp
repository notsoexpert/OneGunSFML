#pragma once

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

    entt::entity Create(entt::registry &registry, 
        Type type, const sf::Texture &texture, 
        const sf::Vector2f& position, const sf::Vector2f& direction, 
        entt::entity source);


    struct Data {
        sf::IntRect TextureRect;
        sf::IntRect CollisionRect;
        float Speed;
        float Lifetime;
        bool IsHoming;
    }; 

    inline constexpr std::array<Data, static_cast<size_t>(Type::Total)> Presets = {
        Data{ sf::IntRect({64, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 10.0f, 1.0f, false}, // Bullet1
        Data{ sf::IntRect({128, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 10.0f, 1.0f, false}, // Bullet2
        Data{ sf::IntRect({192, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 10.0f, 1.0f, false}, // Bullet3
        Data{ sf::IntRect({256, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f, false}, // Laser1
        Data{ sf::IntRect({320, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f, false}, // Laser2
        Data{ sf::IntRect({384, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f, false}, // Laser3
        Data{ sf::IntRect({448, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 4.0f, 1.5f, false}, // Plasma
        Data{ sf::IntRect({576, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 3.0f, false}, // Missile
        Data{ sf::IntRect({0, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 5.0f, 6.0f, true}, // HomingMissile
        Data{ sf::IntRect({64, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 2.5f, 2.0f, false}  // Bomb
    };
}
#pragma once

#include "system/constants.hpp"

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

    void OnCollision(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity);
    
    struct Config {
        sf::IntRect TextureRect;
        sf::IntRect CollisionRect;
        float Speed;
        float Lifetime;
        bool IsHoming;
        bool IsExploding;
        bool IsSplitting;
        bool IsBurning;
        bool IsDestructing = true;
    }; 

    inline constexpr std::array<Config, static_cast<size_t>(Type::Total)> Presets = {
        Config{ sf::IntRect({64, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 10.0f, 1.0f, 
        false, false, false, false}, // Bullet1
        Config{ sf::IntRect({128, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 10.0f, 1.0f, 
        false, false, false, false}, // Bullet2
        Config{ sf::IntRect({192, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 10.0f, 1.0f, 
        false, false, false, false}, // Bullet3
        Config{ sf::IntRect({256, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f, 
        false, false, true, false}, // Laser1
        Config{ sf::IntRect({320, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f, 
        false, false, true, false}, // Laser2
        Config{ sf::IntRect({384, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f, 
        false, false, true, false}, // Laser3
        Config{ sf::IntRect({448, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 4.0f, 1.5f, 
        false, false, false, true, false}, // Plasma
        Config{ sf::IntRect({576, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 3.0f, 
        false, true, false, false}, // Missile
        Config{ sf::IntRect({0, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 5.0f, 6.0f, 
        true, true, false, false}, // HomingMissile
        Config{ sf::IntRect({64, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 2.5f, 2.0f, 
        false, true, false, false, false}  // Bomb
    };

    struct Component {
        Type ThisType;

        Component(Type type = Type::Bullet1) : ThisType(type) {}

        [[nodiscard]] bool IsHoming() const {
            return Presets.at(static_cast<size_t>(ThisType)).IsHoming;
        }

        [[nodiscard]] bool IsExploding() const {
            return Presets.at(static_cast<size_t>(ThisType)).IsExploding;
        }

        [[nodiscard]] bool IsSplitting() const {
            return Presets.at(static_cast<size_t>(ThisType)).IsSplitting;
        }

        [[nodiscard]] bool IsBurning() const {
            return Presets.at(static_cast<size_t>(ThisType)).IsBurning;
        }

        [[nodiscard]] bool IsDestructing() const {
            return Presets.at(static_cast<size_t>(ThisType)).IsDestructing;
        }
    };
}
#pragma once

#include "system/constants.hpp"

namespace Explosion {
    // Isn't this just a projectile without Destructing? I don't know how to structure anything

    enum Type {
        AsteroidDeath = 0,
        ShipDeath,
        MinibossDeath,
        GalaxisDeath,
        PlayerDeath,
        Missile,
        Bomb,
        Total
    };

    struct Component {
        Type ThisType;

        Component(Type type) : ThisType(type) {}
    };

    entt::entity Create(entt::registry& registry, Type type, const sf::Vector2f position,
        const sf::Vector2f velocity, entt::entity source);

    struct Specification {
        OneGunGame::Images TextureID;
        sf::IntRect TextureRect;
        sf::IntRect CollisionRect;
        float Damage;
    };

    inline constexpr std::array<Specification, static_cast<size_t>(Type::Total)> Specifications {
        Specification{OneGunGame::Images::ExplosionRed, 
            {{64, 64}, {64, 64}}, {{64, 64},{64, 64}}, 0.0f},
        Specification{OneGunGame::Images::ExplosionViolet, 
            {{64, 64}, {64, 64}}, {{64, 64},{64, 64}}, 0.0f},
        Specification{OneGunGame::Images::ExplosionViolet, 
            {{64, 64}, {64, 64}}, {{64, 64},{64, 64}}, 0.0f},
        Specification{OneGunGame::Images::ExplosionViolet, 
            {{64, 64}, {64, 64}}, {{64, 64},{64, 64}}, 0.0f},
        Specification{OneGunGame::Images::ExplosionBlue, 
            {{64, 64}, {64, 64}}, {{64, 64},{64, 64}}, 0.0f},
        Specification{OneGunGame::Images::ExplosionYellow, 
            {{64, 64}, {64, 64}}, {{64, 64},{64, 64}}, 10.0f},
        Specification{OneGunGame::Images::ExplosionGreen, 
            {{64, 64}, {64, 64}}, {{64, 64},{64, 64}}, 100.0f}
    };

    void OnCollision(entt::registry &registry, entt::entity explosionEntity, entt::entity otherEntity);
    float GetExplosionDamage(entt::registry &registry, entt::entity explosionEntity, const Component& component);
}
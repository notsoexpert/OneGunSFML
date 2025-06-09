#pragma once

#include "system/constants.hpp"
#include "entities/explosion.hpp"

namespace Explosion {
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

    entt::entity Create(Setup& setup, Type type);

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
}
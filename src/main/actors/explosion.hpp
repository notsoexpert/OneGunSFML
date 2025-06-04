#pragma once

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

    entt::entity Create(entt::registry& registry, Type type, const sf::Vector2f position,
        const sf::Vector2f velocity);

    struct Specification {
        sf::IntRect TextureRect;
        sf::IntRect CollisionRect;
    };

    inline constexpr std::array<Specification, static_cast<size_t>(Type::Total)> Specifications {
        Specification{{{64, 64}, {64, 64}}, {{64, 64},{64, 64}}},
        Specification{{{64, 64}, {64, 64}}, {{64, 64},{64, 64}}}
    };
}
#include "pch.hpp"
#include "entities/enemy_types.hpp"

#include "entities/entity.hpp"
#include "entities/explosion_types.hpp"

#include "components/renderable.hpp"
#include "components/transformation.hpp"
#include "components/lifetime.hpp"

namespace OneGunGame{
namespace Enemy::Asteroid {
    static constexpr const char* Name = "Asteroid";
    static constexpr Images ImageID = Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{0, 64}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {16, 16}};
    static constexpr float MaxHealth = 10.0f;
    static constexpr float MoveSpeed = 2.5f;
    static constexpr float OffscreenLifetime = 3.0f;

    void Create(const Setup& setup){
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        SetupHealth(setup, MaxHealth, Death);
        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);

        setup.Registry.emplace<Rotating>(setup.ThisEntity, sf::radians(2*Pi));
    }

    void Death(entt::registry &registry, entt::entity thisEntity){
        registry.emplace<Destructing>(thisEntity);

        Explosion::Setup explosionSetup = Explosion::SetupBasicExplosion(registry,
        registry.get<Renderable>(thisEntity).Sprite.getPosition(),
        registry.get<Velocity>(thisEntity).Value);
        Explosion::AsteroidDeath::Create(explosionSetup);
        registry.get<Renderable>(explosionSetup.ThisEntity).Sprite.setScale({0.75f, 0.75f});
    }
}
}
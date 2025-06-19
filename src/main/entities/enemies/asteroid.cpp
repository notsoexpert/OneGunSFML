#include "pch.hpp"
#include "entities/enemy_types.hpp"

#include "system/components.hpp"
#include "entities/entity.hpp"
#include "entities/explosion_types.hpp"

namespace Enemy::Asteroid {
    static constexpr const char* Name = "Asteroid";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
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
        auto& health = SetupHealth(setup, MaxHealth);
        health.OnDeath = Death;
        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);

        setup.Registry.emplace<Rotating>(setup.ThisEntity, sf::radians(2*OneGunGame::Pi));
    }

    void Death(entt::registry &registry, entt::entity thisEntity){
        registry.emplace<Destructing>(thisEntity);

        Explosion::Setup explosionSetup{
            registry,
            registry.get<Renderable>(thisEntity).Sprite.getPosition(),
            registry.get<Velocity>(thisEntity).Value,
            registry.get<Collidable>(thisEntity).Source
        };
        Explosion::VisualOnly::AsteroidDeath::Create(explosionSetup);
        registry.get<Renderable>(explosionSetup.ThisEntity).Sprite.setScale({0.75f, 0.75f});
    }
}

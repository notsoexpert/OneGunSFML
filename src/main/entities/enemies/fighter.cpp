#include "pch.hpp"
#include "entities/enemy_types.hpp"

#include "entities/entity.hpp"

namespace Enemy::Fighter {
    static constexpr const char* Name = "Fighter";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{256, 64}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {48, 48}};
    static constexpr float MaxHealth = 40.0f;
    static constexpr float MoveSpeed = 5.0f;
    static constexpr float OffscreenLifetime = 5.0f;

    void Create(const Setup& setup){
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        SetupHealth(setup, MaxHealth, Death);
        SetupBehavior(setup, Behavior);
        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);
    }

    void Behavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity)){
            spdlog::trace("Fighter Behavior invoked for entity {}", static_cast<int>(thisEntity));
        }
    }

    void Death(entt::registry &registry, entt::entity thisEntity){
        registry.emplace<Destructing>(thisEntity);

        /* CREATE DEATH EXPLOSION */
        // auto &renderable = registry.get<Renderable>(thisEntity);
        // auto &velocity = registry.get<Velocity>(thisEntity);
        // Explosion::Setup setup{registry, renderable.Sprite.getPosition(), velocity.Value};
        // Explosion::Create(setup, Explosion::Type::AsteroidDeath);
    }
}
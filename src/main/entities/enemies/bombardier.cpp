#include "pch.hpp"
#include "entities/enemy_types.hpp"
//#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"

#include "components/lifetime.hpp"

namespace OneGunGame{
namespace Enemy::Bombardier {
    static constexpr const char* Name = "Bombardier";
    static constexpr Images ImageID = Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{0, 64}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {16, 16}};
    static constexpr float MaxHealth = 300.0f;
    static constexpr float MoveSpeed = 5.0f;
    static constexpr float OffscreenLifetime = 6.0f;

    void Create(const Setup& setup){
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        SetupHealth(setup, MaxHealth, Death);
        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);

        setup.Registry.emplace<Enemy::Behavior>(setup.ThisEntity, Behavior);
    }

    void Behavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity)){
            spdlog::trace("Bombardier Behavior invoked for entity {}", static_cast<int>(thisEntity));
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
}
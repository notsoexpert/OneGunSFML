#include "pch.hpp"
#include "entities/enemy_types.hpp"

#include "entities/projectile_types.hpp"
#include "system/components.hpp"
#include "entities/entity.hpp"

namespace Enemy::Drone {
    static constexpr const char* Name = "Drone";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{192, 64}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {48, 48}};
    static constexpr float MaxHealth = 20.0f;
    static constexpr float MoveSpeed = 2.0f;
    static constexpr float OffscreenLifetime = 5.0f;
    static constexpr float FireDamage = 1.0f;
    static constexpr float FireRate = 1.0f;

    void Create(const Setup& setup){
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        auto& health = SetupHealth(setup, MaxHealth);
        health.OnDeath = Death;
        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);
        setup.Registry.emplace<Enemy::Behavior>(setup.ThisEntity, Behavior);
        setup.Registry.emplace<Fireable>(setup.ThisEntity, FireDamage, FireRate);
        setup.Registry.emplace<Projectile::Weapon>(setup.ThisEntity, Projectile::Weapon::Cannon);

    }

    void Behavior(entt::registry &registry, entt::entity thisEntity){
        Fire(registry, thisEntity);
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
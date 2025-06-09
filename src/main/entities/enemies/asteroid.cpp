#include "pch.hpp"
#include "entities/enemy.hpp"

#include "system/components.hpp"
#include "entities/entity.hpp"

namespace Enemy {
    static constexpr const char* Name = "Asteroid";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{0, 64}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{-8, -8}, {16, 16}};
    static constexpr float MaxHealth = 10.0f;
    static constexpr float MoveSpeed = 2.5f;
    static constexpr float OffscreenLifetime = 3.0f;

    void AsteroidSetup(const Setup& setup){
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        auto& health = SetupHealth(setup, MaxHealth);
        health.OnDeath = AsteroidDeath;
        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);

        setup.Registry.emplace<Rotating>(setup.ThisEntity, sf::radians(2*OneGunGame::Pi));
    }

    void AsteroidDeath(entt::registry &registry, entt::entity thisEntity){
        registry.emplace<Destructing>(thisEntity);

        /* CREATE DEATH EXPLOSION */
        //auto &renderable = registry.get<Renderable>(thisEntity);
        //auto &velocity = registry.get<Velocity>(thisEntity);
        //Explosion::Setup setup{registry, renderable.Sprite.getPosition(), velocity.Value};
        //Explosion::Create(setup, Explosion::Type::AsteroidDeath);
    }
}
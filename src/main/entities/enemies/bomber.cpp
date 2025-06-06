#include "pch.hpp"
#include "entities/enemy.hpp"

namespace Enemy {
    static constexpr const char* Name = "Bomber";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{320, 64}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{-24, -24}, {48, 48}};
    static constexpr float MaxHealth = 50.0f;
    static constexpr float MoveSpeed = 3.0f;
    static constexpr float OffscreenLifetime = 5.0f;

    void BomberSetup(const Setup& setup){
        
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        auto& health = SetupHealth(setup, MaxHealth);
        health.OnDeath = BomberDeath;
        SetupOffscreenLifetime(setup, OffscreenLifetime);
    }

    void BomberBehavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity)){
            spdlog::trace("Bomber Behavior invoked for entity {}", static_cast<int>(thisEntity));
        }
    }

    void BomberDeath(entt::registry &registry, entt::entity thisEntity){
        registry.emplace<Destructing>(thisEntity);

        /* CREATE DEATH EXPLOSION */
        // auto &renderable = registry.get<Renderable>(thisEntity);
        // auto &velocity = registry.get<Velocity>(thisEntity);
        // Explosion::Setup setup{registry, renderable.Sprite.getPosition(), velocity.Value};
        // Explosion::Create(setup, Explosion::Type::AsteroidDeath);
    }
}
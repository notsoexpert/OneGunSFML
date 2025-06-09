#include "pch.hpp"
#include "entities/enemy_types.hpp"

#include "entities/entity.hpp"

namespace Enemy {
    static constexpr const char* Name = "Hunter";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{384, 64}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {64, 64}};
    static constexpr float MaxHealth = 150.0f;
    static constexpr float MoveSpeed = 6.0f;
    static constexpr float OffscreenLifetime = 3.0f;

    void HunterSetup(const Setup& setup){
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        auto& health = SetupHealth(setup, MaxHealth);
        health.OnDeath = HunterDeath;
        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);

        setup.Registry.emplace<Behavior>(setup.ThisEntity, HunterBehavior);
    }

    void HunterBehavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity)){
            spdlog::trace("Hunter Behavior invoked for entity {}", static_cast<int>(thisEntity));
        }
    }

    void HunterDeath(entt::registry &registry, entt::entity thisEntity){
        registry.emplace<Destructing>(thisEntity);

        /* CREATE DEATH EXPLOSION */
        //auto &renderable = registry.get<Renderable>(thisEntity);
        //auto &velocity = registry.get<Velocity>(thisEntity);
        //Explosion::Setup setup{registry, renderable.Sprite.getPosition(), velocity.Value};
        //Explosion::Create(setup, Explosion::Type::AsteroidDeath);
    }
}
#include "pch.hpp"
#include "entities/enemy_types.hpp"

#include "entities/entity.hpp"
#include "entities/explosion_types.hpp"

namespace Enemy::LargeAsteroid {
    static constexpr const char* Name = "Large Asteroid";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{64, 64}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {46, 46}};
    static constexpr float MaxHealth = 25.0f;
    static constexpr float MoveSpeed = 1.0f;
    static constexpr float OffscreenLifetime = 5.0f;
    static constexpr size_t DeathAsteroids = 2U;

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
        
        auto &renderable = registry.get<Renderable>(thisEntity);
        auto &velocity = registry.get<Velocity>(thisEntity);

        for (auto i : std::ranges::iota_view{0U, DeathAsteroids}) {
            sf::Vector2f newDirection = velocity.Value.rotatedBy(sf::radians(OneGunGame::HalfPi / 2.0f * (i / 2 + 1) * (i % 2 == 0 ? 1.0f : -1.0f))).normalized();
            Enemy::Setup setup{registry, renderable.Sprite.getPosition(), newDirection, entt::null, thisEntity};
            Enemy::Create(setup, Enemy::Type::Asteroid);
        }

        Explosion::Setup explosionSetup{
            registry,
            registry.get<Renderable>(thisEntity).Sprite.getPosition(),
            registry.get<Velocity>(thisEntity).Value,
            registry.get<Collidable>(thisEntity).Source
        };
        Explosion::AsteroidDeath::Create(explosionSetup);
        registry.get<Renderable>(explosionSetup.ThisEntity).Sprite.setScale({1.25f, 1.25f});
    }
}
#include "pch.hpp"
#include "entities/enemy_types.hpp"

#include "entities/entity.hpp"

namespace Enemy {
    static constexpr const char* Name = "Huge Asteroid";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{128, 64}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{-32, -32}, {64, 64}};
    static constexpr float MaxHealth = 50.0f;
    static constexpr float MoveSpeed = 0.5f;
    static constexpr float OffscreenLifetime = 5.0f;
    static constexpr int DeathLargeAsteroids = 2;

    void HugeAsteroidSetup(const Setup& setup){
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        auto& health = SetupHealth(setup, MaxHealth);
        health.OnDeath = HugeAsteroidDeath;
        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);

        setup.Registry.emplace<Rotating>(setup.ThisEntity, sf::radians(2*OneGunGame::Pi));
    }

    void HugeAsteroidDeath(entt::registry &registry, entt::entity thisEntity){
        registry.emplace<Destructing>(thisEntity);

        /* GET RELEVANT COMPONENTS */
        auto &renderable = registry.get<Renderable>(thisEntity);
        auto &velocity = registry.get<Velocity>(thisEntity);

        /* SPLIT INTO ASTEROIDS */
        for (int i = 0; i < DeathLargeAsteroids; i++) {
            sf::Vector2f newDirection = velocity.Value.rotatedBy(sf::radians(OneGunGame::HalfPi / 2.0f * (i/2+1) * (i%2==0?1.0f:-1.0f))).normalized();
            Enemy::Setup setup{registry, renderable.Sprite.getPosition(), newDirection, entt::null, thisEntity};
            Enemy::Create(setup, Enemy::Type::LargeAsteroid);
        }

        /* CREATE DEATH EXPLOSION */
        //Explosion::Setup setup{registry, renderable.Sprite.getPosition(), velocity.Value};
        //Explosion::Create(setup, Explosion::Type::AsteroidDeath);
    }
}
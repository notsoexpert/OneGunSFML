#include "pch.hpp"
#include "entities/enemy.hpp"

namespace Enemy {
    static constexpr const char* Name = "Comet";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{352, 480}, {32, 32}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {64, 64}};
    static constexpr float MaxHealth = 100.0f;
    static constexpr float MoveSpeed = 6.0f;
    static constexpr float OffscreenLifetime = 2.5f;
    static constexpr int DeathProjectiles = 8;

    void CometSetup(const Setup& setup){
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        auto& health = SetupHealth(setup, MaxHealth);
        health.OnDeath = CometDeath;
        SetupOffscreenLifetime(setup, OffscreenLifetime);

        setup.Registry.emplace<Behavior>(setup.ThisEntity, CometBehavior);
    }

    void CometBehavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity)){
            spdlog::trace("Comet Behavior invoked for entity {}", static_cast<int>(thisEntity));
        }
    }

    void CometDeath(entt::registry &registry, entt::entity thisEntity){
        registry.emplace<Destructing>(thisEntity);

        /* DEATH STUFF */
        //auto &renderable = registry.get<Renderable>(thisEntity);
        //auto &velocity = registry.get<Velocity>(thisEntity);

        /* LAUNCH ICE PROJECTILES */
        //for (int i = 0; i < DeathProjectiles; i++) {
            //sf::Vector2f direction = velocity.Value.rotatedBy((OneGunGame::PI * 2) / (DeathProjectiles==0?1:DeathProjectiles) * i);
            //Projectile::Setup setup{registry, renderable.Sprite.getPosition(), direction, entt::null, thisEntity};
            //Projectile::Create(setup, Projectile::Type::IceShard);
        //}

        /* CREATE DEATH EXPLOSION */
        //Explosion::Setup setup{registry, renderable.Sprite.getPosition(), velocity.Value};
        //Explosion::Create(setup, Explosion::Type::AsteroidDeath);
    }
}
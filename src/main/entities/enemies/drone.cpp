#include "pch.hpp"
#include "entities/enemy_types.hpp"

#include "entities/projectile_types.hpp"
#include "system/components.hpp"
#include "entities/entity.hpp"
#include "entities/weapon.hpp"
#include "entities/explosion_types.hpp"
#include "system/onegungame.hpp"

namespace Enemy::Drone {
    static constexpr const char* Name = "Drone";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{192, 64}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {48, 48}};
    static constexpr float MaxHealth = 20.0f;
    static constexpr float MoveSpeed = 2.5f;
    static constexpr float OffscreenLifetime = 5.0f;

    static constexpr float FireDamage = 1.0f;
    static constexpr float FireRate = 1.0f;

    static constexpr sf::Vector2f BaseForwardVector = {0.0f, 1.0f};

    void Create(const Setup& setup){
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        auto& health = SetupHealth(setup, MaxHealth);
        health.OnDeath = Death;
        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);
        setup.Registry.emplace<Enemy::Behavior>(setup.ThisEntity, Behavior);
        setup.Registry.emplace<Fireable>(setup.ThisEntity);

        auto &weapon = setup.Registry.emplace<Weapon::Component>(
            setup.ThisEntity, 
            Weapon::Type::DroneCannon
        );
        weapon.SetBaseStats(FireDamage, FireRate);
        weapon.ForwardVector = BaseForwardVector;

    }

    void Behavior(entt::registry &registry, entt::entity thisEntity){
        auto &renderable = registry.get<Renderable>(thisEntity);
        
        // helper function probably needed
        sf::IntRect drone = renderable.Sprite.getTextureRect();
        drone.position = OneGunGame::RoundVector(renderable.Sprite.getPosition());
        drone.size = OneGunGame::RoundVector(static_cast<sf::Vector2f>(drone.size).componentWiseMul(renderable.Sprite.getScale()));
        sf::IntRect window = {{0,0}, sf::Vector2i(OneGunGame::GetWindowSize())};
        if (!drone.findIntersection(window)){
            return;
        }

        Projectile::Fire(registry, thisEntity);

        auto playerEntity = OneGunGame::GetPlayerEntity();

        if (!registry.valid(playerEntity)){
            spdlog::warn("Drone::Behavior - player entity invalid");
            return;
        }

        auto playerRenderable = registry.try_get<Renderable>(playerEntity);
        if (!playerRenderable) {
            spdlog::warn("Drone::Behavior - player entity missing Renderable component");
            return;
        }
        
        renderable.Sprite.setRotation((playerRenderable->Sprite.getPosition() - renderable.Sprite.getPosition()).angle() - sf::radians(OneGunGame::HalfPi));
    }

    void Death(entt::registry &registry, entt::entity thisEntity){
        registry.emplace<Destructing>(thisEntity);

        Explosion::Setup explosionSetup{
            registry,
            registry.get<Renderable>(thisEntity).Sprite.getPosition(),
            registry.get<Velocity>(thisEntity).Value
        };
        Explosion::DroneDeath::Create(explosionSetup);
        registry.get<Renderable>(explosionSetup.ThisEntity).Sprite.setScale({2.0f, 2.0f});
    }
}
#include "pch.hpp"
#include "player.hpp"

#include "systems/onegungame.hpp"
#include "entities/projectile_types.hpp"
#include "entities/entity.hpp"

#include "components/renderable.hpp"
#include "components/lifetime.hpp"
#include "components/transformation.hpp"
#include "components/mechanics.hpp"
#include "components/collidable.hpp"
#include "components/weapon.hpp"

namespace OneGunGame{
namespace Player {
    constexpr float BaseMaxHealth = 100.0f;
    constexpr float BaseMaxEnergy = 100.0f;
    constexpr float BaseMoveSpeed = 5.0f;
    constexpr float BaseAcceleration = 1.5f;
    constexpr float BaseFriction = 0.15f;
    constexpr float BaseHitInvincibilityDuration = 1.0f;
    constexpr float BaseDamage = 5.0f;
    constexpr float BaseFireRate = 4.0f;
    constexpr float BaseShotSpeed = 1.0f;
    constexpr float BaseDashSpeedMultiplier = 5.0f;
    constexpr float BaseDashDuration = 0.25f;
    constexpr float BaseDashCooldown = 0.8f;
    constexpr std::array<sf::Vector2f, 3> DashAccelerationCurve = {
        sf::Vector2f{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}
    };

    void Update(entt::registry &registry, entt::entity playerEntity) {
        sf::Vector2f inputVector = GetInputVector();
        Player::Move(inputVector, registry, playerEntity);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            Entity::StartDash(registry.get<Dashable>(playerEntity), registry.get<Velocity>(playerEntity));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
            Projectile::Fire(registry, playerEntity);
        }
    }

    entt::entity Create(entt::registry &registry, const sf::Vector2f &startPosition) {
        entt::entity entity = registry.create();
        auto &renderable = registry.emplace<Renderable>(entity, GetTexture(Images::SpriteSheet), 10);
        renderable.Sprite.setOrigin({Size.x / 2.0f, Size.y / 2.0f}); // Set origin to center of the sprite
        renderable.Sprite.setPosition(startPosition);
        renderable.Sprite.setTextureRect(sf::IntRect({0, 0}, Size));
        
        registry.emplace<Collidable>(
            entity, 
            sf::IntRect{CollisionOffset, CollisionSize}, 
            entity,
            CollisionLayer::Player, 
            GetCollisionMask({
                CollisionLayer::Enemy, CollisionLayer::EnemyProjectile,
                CollisionLayer::Obstacle, CollisionLayer::NeutralProjectile
            }),
            OnCollision
        );
        registry.emplace<Velocity>(entity);
        registry.emplace<Acceleration>(entity);
        registry.emplace<Friction>(entity, BaseFriction);
        registry.emplace<Health>(entity, BaseMaxHealth, OnDeath);
        registry.emplace<Energy>(entity, BaseMaxEnergy);
        registry.emplace<MaxSpeed>(entity, BaseMoveSpeed);
        registry.emplace<HitInvincibility>(entity, BaseHitInvincibilityDuration);
        auto &weapon = registry.emplace<Weapon>(entity, Weapon::Type::MainCannon);
        weapon.SetBaseStats(BaseDamage, BaseFireRate, BaseShotSpeed);

        registry.emplace<Dashable>(entity, 
            DashAccelerationCurve, 
            BaseDashSpeedMultiplier, 
            BaseDashDuration, 
            BaseDashCooldown
        );
        registry.emplace<Confined>(entity, 
            sf::FloatRect{sf::Vector2f{}, static_cast<sf::Vector2f>(GetWindowSize())}
        );
        
        return entity;
    }

    void Move(const sf::Vector2f &inputVector, entt::registry &registry, entt::entity playerEntity) {
        // auto &dashable = registry.get<Dashable>(playerEntity);
        // if (dashable.CurrentState != Dashable::DashState::None) {
        //     return;
        // }

        auto &acceleration = registry.get<Acceleration>(playerEntity);
        
        acceleration.Value = sf::Vector2f{
            inputVector.x * Player::BaseAcceleration,
            inputVector.y * Player::BaseAcceleration
        };
    }

    void OnCollision(Collision& collision) {
        spdlog::info("Player (entity {}) collided with entity {}", 
            static_cast<int>(collision.ThisEntity), static_cast<int>(collision.OtherEntity));

        auto &hitInvincibility = collision.Registry.get<HitInvincibility>(collision.ThisEntity);
        if (hitInvincibility.IsInvincible()) {
            return;
        }

        auto &health = collision.Registry.get<Health>(collision.ThisEntity);
        // TEMP
        if (health.Current <= 0.0f) {
            spdlog::warn("You dead");
            return;
        }
    }

    void OnDeath(entt::registry &registry, entt::entity playerEntity) {
        //registry.emplace<Destructing>(playerEntity);
        auto& health = registry.get<Health>(playerEntity);
        spdlog::warn("Player died! Healing to full :)");
        health.Heal(health.Max);
    }
}
}
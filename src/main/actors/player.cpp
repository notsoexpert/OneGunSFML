#include "pch.hpp"
#include "player.hpp"

#include "system/onegungame.hpp"
#include "components/components.hpp"
#include "actors/projectile.hpp"

namespace Player {
    entt::entity Fire(entt::registry &registry, entt::entity playerEntity, const sf::Texture& projectileTexture) {
        auto fireComponent = registry.try_get<Fireable>(playerEntity);
        if (!fireComponent) {
            spdlog::warn("Player entity {} does not have a Fireable component", static_cast<int>(playerEntity));
            return entt::null;
        }

        if (fireComponent->Fire()) {
            spdlog::trace("Player firing projectile");
            auto projectileType = static_cast<Projectile::Type>(
                OneGunGame::GetRandomGenerator().generateInt(0, static_cast<int>(Projectile::Type::Total) - 1));
            spdlog::trace("Projectile type: {}", static_cast<int>(projectileType));
            auto projectile = Projectile::Create(registry, 
                projectileType, projectileTexture, 
                registry.get<Renderable>(playerEntity).Sprite.getPosition(), sf::Vector2f{0.0f, -1.0f}, 
                playerEntity);
            return projectile;
        }
        return entt::null;
    }

    entt::entity Create(entt::registry &registry, const sf::Texture &texture, const sf::Vector2f &startPosition) {
        entt::entity entity = registry.create();
        auto &renderable = registry.emplace<Renderable>(entity, texture, 10);
        renderable.Sprite.setOrigin({Player::Size.x / 2.0f, Player::Size.y / 2.0f}); // Set origin to center of the sprite
        renderable.Sprite.setPosition(startPosition);
        renderable.Sprite.setTextureRect(sf::IntRect({0, 0}, Player::Size));
        
        registry.emplace<Collidable>(entity, sf::IntRect{Player::CollisionOffset, Player::CollisionSize}, entity,
            OneGunGame::CollisionLayer::Player,
            static_cast<OneGunGame::CollisionLayer>(OneGunGame::Enemy | OneGunGame::Projectile | OneGunGame::Obstacle),
            OnCollision);
        registry.emplace<Velocity>(entity, 0.0f, 0.0f);

        registry.emplace<Health>(entity, BaseMaxHealth);
        registry.emplace<Energy>(entity, BaseMaxEnergy);
        registry.emplace<Speed>(entity, Player::BaseMoveSpeed, Player::BaseAcceleration);
        registry.emplace<HitInvincibility>(entity, Player::BaseHitInvincibilityDuration);
        registry.emplace<Fireable>(entity, Player::BaseDamage, Player::BaseFireRate);
        registry.emplace<Dashable>(entity, Player::BaseDashSpeedMultiplier, Player::BaseDashDuration, Player::BaseDashCooldown);
        
        return entity;
    }

    void Move(const sf::Vector2f &inputVector, entt::registry &registry, entt::entity playerEntity) {
        auto &velocity = registry.get<Velocity>(playerEntity);
        auto &speed = registry.get<Speed>(playerEntity);
        
        auto acceleration = sf::Vector2f{
            inputVector.x * speed.MoveAcceleration,
            inputVector.y * speed.MoveAcceleration
        };

        velocity += acceleration;
        velocity.x = std::clamp(velocity.x, -speed.MoveMaxSpeed, speed.MoveMaxSpeed);
        velocity.y = std::clamp(velocity.y, -speed.MoveMaxSpeed, speed.MoveMaxSpeed);
    }

    void Dash(entt::registry &registry, entt::entity playerEntity) {
        auto &dashable = registry.get<Dashable>(playerEntity);
        if (dashable.IsDashCooldownComplete()) {
            // TBD: Do a dash
        }
    }

    void OnCollision(entt::registry &registry, entt::entity playerEntity, entt::entity otherEntity) {
        spdlog::info("Player (entity {}) collided with entity {}", 
            static_cast<int>(playerEntity), static_cast<int>(otherEntity));

        auto &hitInvincibility = registry.get<HitInvincibility>(playerEntity);
        if (hitInvincibility.IsInvincible()) {
            return;
        }

        auto &health = registry.get<Health>(playerEntity);
        // TEMP
        if (health.Current <= 0.0f) {
            spdlog::warn("You dead");
            return;
        }
    }
}
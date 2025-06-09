#include "pch.hpp"
#include "player.hpp"

#include "system/onegungame.hpp"
#include "system/components.hpp"
#include "entities/projectile_types.hpp"

namespace Player {

    void Update(entt::registry &registry, entt::entity playerEntity) {
        sf::Vector2f inputVector = OneGunGame::GetInputVector();
        Player::Move(inputVector, registry, playerEntity);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            Player::Dash(registry, playerEntity);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
            Player::Fire(registry, playerEntity);
        }
    }

    entt::entity Create(entt::registry &registry, const sf::Vector2f &startPosition) {
        entt::entity entity = registry.create();
        auto &renderable = registry.emplace<Renderable>(entity, OneGunGame::GetTexture(OneGunGame::SpriteSheet), 10);
        renderable.Sprite.setOrigin({Player::Size.x / 2.0f, Player::Size.y / 2.0f}); // Set origin to center of the sprite
        renderable.Sprite.setPosition(startPosition);
        renderable.Sprite.setTextureRect(sf::IntRect({0, 0}, Player::Size));
        
        registry.emplace<Collidable>(entity, sf::IntRect{Player::CollisionOffset, Player::CollisionSize}, entity,
            OneGunGame::CollisionLayer::Player,
            static_cast<OneGunGame::CollisionLayer>(OneGunGame::Enemy | OneGunGame::Projectile | OneGunGame::Obstacle),
            OnCollision);
        registry.emplace<Velocity>(entity);
        registry.emplace<Acceleration>(entity);
        registry.emplace<Friction>(entity, Player::BaseFriction);
        registry.emplace<Health>(entity, Player::BaseMaxHealth, OnDeath);
        registry.emplace<Energy>(entity, Player::BaseMaxEnergy);
        registry.emplace<MaxSpeed>(entity, Player::BaseMoveSpeed);
        registry.emplace<HitInvincibility>(entity, Player::BaseHitInvincibilityDuration);
        registry.emplace<Fireable>(entity, Player::BaseDamage, Player::BaseFireRate);
        registry.emplace<Projectile::Weapon>(entity, Projectile::Weapon::Cannon);
        registry.emplace<Dashable>(entity, Player::BaseDashSpeedMultiplier, Player::BaseDashDuration, Player::BaseDashCooldown);
        registry.emplace<Confined>(entity, sf::FloatRect{sf::Vector2f{}, static_cast<sf::Vector2f>(OneGunGame::GetWindowSize())});
        
        return entity;
    }

    void Move(const sf::Vector2f &inputVector, entt::registry &registry, entt::entity playerEntity) {
        auto &acceleration = registry.get<Acceleration>(playerEntity);
        
        acceleration.Value = sf::Vector2f{
            inputVector.x * Player::BaseAcceleration,
            inputVector.y * Player::BaseAcceleration
        };
    }

    entt::entity Fire(entt::registry &registry, entt::entity playerEntity) {
        auto fireComponent = registry.try_get<Fireable>(playerEntity);
        if (!fireComponent) {
            spdlog::warn("Player entity {} does not have a Fireable component", static_cast<int>(playerEntity));
            return entt::null;
        }

        if (!fireComponent->Fire())
            return entt::null;

        auto weaponComponent = registry.try_get<Projectile::Weapon>(playerEntity);
        if (!weaponComponent) {
            spdlog::warn("Player entity {} does not have a Weapon component", static_cast<int>(playerEntity));
            return entt::null;
        }

        spdlog::trace("Player firing projectile");
        auto projectileType = weaponComponent->GetBulletType();
        spdlog::trace("Projectile type: {}", static_cast<int>(projectileType));
        Projectile::Setup setup{registry, registry.get<Renderable>(playerEntity).Sprite.getPosition(), 
            sf::Vector2f{0.0f, -1.0f}, playerEntity};
        auto projectile = Projectile::Create(setup, projectileType);
        return projectile;
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

    void OnDeath(entt::registry &registry, entt::entity playerEntity) {
        registry.emplace<Destructing>(playerEntity);
    }
}
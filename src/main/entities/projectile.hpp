#pragma once

#include "systems/constants.hpp"

namespace OneGunGame{
struct Renderable;
struct Collision;
struct Collidable;
struct Health;

namespace Projectile {
    struct Setup {
        entt::registry &Registry;
        const sf::Vector2f &Position;
        const sf::Vector2f &Direction;
        std::optional<CollisionLayer> CollisionLayer;
        std::optional<uint8_t> CollisionMask;
        entt::entity Source = entt::null;
        entt::entity ThisEntity = entt::null;
        uint8_t Tier = 0U;
    };

    void Update(entt::registry& registry);
    entt::entity Create(Setup& setup);
    void OnCollision(Collision& collision);
    
    Renderable& SetupRenderable(const Setup& setup, 
        Images imageID, const sf::IntRect& textureRect);
    Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect);
    void SetupMovement(const Setup& setup, float moveSpeed);
        
    float GetProjectileDamage(entt::registry &registry, entt::entity projectileEntity, float damage);

    struct Splitting {
        size_t MaxSplits;
        size_t CurrentSplits = 0;
        sf::Vector2f OriginalDirection;

        Splitting(size_t maxSplits = 2, const sf::Vector2f& originalDirection = {0.0f, 1.0f}) : MaxSplits(maxSplits), OriginalDirection(originalDirection) {}
        
        [[nodiscard]] bool CanSplit() const {
            return CurrentSplits < MaxSplits;
        }
    };

    struct Burning {
        float BurnPeriodSeconds = 0.25f;
        std::unordered_map<entt::entity, sf::Clock> BurnClocks;

        [[nodiscard]] bool CanBurn(entt::entity other) const {
            return !BurnClocks.contains(other) || 
                    BurnClocks.at(other).getElapsedTime() >= sf::seconds(BurnPeriodSeconds);
        }
    };

    struct Homing {
        entt::entity Target = entt::null;
        float RotationForce = 1.0f;
    };

    void BurnEntity(entt::registry& registry, entt::entity projectileEntity, entt::entity otherEntity, float damage, Burning& burningComponent);
}
}
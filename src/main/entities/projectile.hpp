#pragma once

#include "system/constants.hpp"

struct Renderable;
struct Collision;
struct Collidable;
struct Health;

namespace Projectile {
    struct Setup {
        entt::registry &Registry;
        const sf::Vector2f &Position;
        const sf::Vector2f &Direction;
        entt::entity Source = entt::null;
        entt::entity ThisEntity = entt::null;
    };

    entt::entity Create(Setup& setup);
    void OnCollision(Collision& collision);
    
    Renderable& SetupRenderable(const Setup& setup, 
        OneGunGame::Images imageID, const sf::IntRect& textureRect);
        Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect);
        
    float GetProjectileDamage(entt::registry &registry, entt::entity projectileEntity, float damage);

    struct Splitting {
        size_t MaxSplits;
        size_t CurrentSplits = 0;

        Splitting(size_t maxSplits = 2) : MaxSplits(maxSplits) {}
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

    void BurnEntity(entt::registry& registry, entt::entity projectileEntity, entt::entity otherEntity, float damage, Burning& burningComponent);
}
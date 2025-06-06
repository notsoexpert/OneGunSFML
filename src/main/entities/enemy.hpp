#pragma once

#include "system/constants.hpp"
#include "system/components.hpp"
#include "entities/enemy_types.hpp"

namespace Enemy {

    void Update(entt::registry& registry);

    entt::entity Create(Setup& setup, Type type);
    
    Renderable& SetupRenderable(const Setup& setup, 
        OneGunGame::Images imageID, const sf::IntRect& textureRect);
    Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect);
    Health& SetupHealth(const Setup& setup, float maxHealth);
    void SetupMovement(const Setup& setup, float moveSpeed);
    void SetupOffscreenLifetime(const Setup& setup, float expireTimeInSeconds);

    entt::entity Fire(entt::registry &registry, entt::entity sourceEntity);

    void OnCollision(entt::registry &registry, entt::entity thisEntity, entt::entity otherEntity);

    struct Behavior {
        using Action = std::function<void(entt::registry &registry, entt::entity thisEntity)>;
        Action Act;

        Behavior(Action act) : Act(act) {}

        static const std::unordered_map<Enemy::Type, Action> SetupEnemy;
    };
}
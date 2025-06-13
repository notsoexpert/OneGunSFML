#pragma once

#include "system/constants.hpp"
#include "system/components.hpp"

struct Collision;

namespace Enemy {
    struct Setup {
        entt::registry &Registry;
        sf::Vector2f Position;
        sf::Vector2f Direction;
        entt::entity Source = entt::null;
        entt::entity ThisEntity = entt::null;
    };

    entt::entity Create(Setup& setup);
    entt::entity Fire(entt::registry &registry, entt::entity sourceEntity);
    void Update(entt::registry& registry);

    struct Behavior {
        using Action = std::function<void(entt::registry &registry, entt::entity thisEntity)>;
        Action Act;

        Behavior(Action act) : Act(act) {}
    };
    
    Renderable& SetupRenderable(const Setup& setup, 
        OneGunGame::Images imageID, const sf::IntRect& textureRect);
    Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect);
    Health& SetupHealth(const Setup& setup, float maxHealth);
    void SetupMovement(const Setup& setup, float moveSpeed);
    void SetupOffscreenLifetime(const Setup& setup, float expireTimeInSeconds);
    void OnCollision(Collision& collision);
}
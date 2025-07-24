#pragma once

#include "systems/constants.hpp"
#include "systems/components.hpp"

struct Collision;

namespace Enemy {
    struct Setup {
        entt::registry &Registry;
        sf::Vector2f Position;
        sf::Vector2f Direction;
        std::optional<OneGunGame::CollisionLayer> CollisionLayer;
        std::optional<uint8_t> CollisionMask;
        entt::entity Source = entt::null;
        entt::entity ThisEntity = entt::null;
    };

    entt::entity Create(Setup& setup);
    void Update(entt::registry& registry);

    struct Behavior {
        using Action = std::function<void(entt::registry &registry, entt::entity thisEntity)>;
        Action Act;

        Behavior(Action act) : Act(act) {}
    };
    
    Renderable& SetupRenderable(const Setup& setup, 
        OneGunGame::Images imageID, const sf::IntRect& textureRect);
    Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect);
    Health& SetupHealth(const Setup& setup, float maxHealth, Callback callback = {});
    void SetupMovement(const Setup& setup, float moveSpeed);
    void SetupOffscreenLifetime(const Setup& setup, float expireTimeInSeconds);
    Behavior& SetupBehavior(const Setup& setup, Callback callback = {});
    void OnCollision(Collision& collision);
}
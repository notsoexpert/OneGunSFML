#pragma once

#include "systems/constants.hpp"

namespace OneGunGame {
struct Renderable;
struct Collidable;
struct Collision;
struct Health;

namespace Enemy {
    struct Setup {
        entt::registry &Registry;
        sf::Vector2f Position;
        sf::Vector2f Direction;
        std::optional<CollisionLayer> CLayer;
        std::optional<uint8_t> CMask;
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
        Images imageID, const sf::IntRect& textureRect);
    Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect);
    Health& SetupHealth(const Setup& setup, float maxHealth, Callback callback = {});
    void SetupMovement(const Setup& setup, float moveSpeed);
    void SetupOffscreenLifetime(const Setup& setup, float expireTimeInSeconds);
    Behavior& SetupBehavior(const Setup& setup, Callback callback = {});
    void OnCollision(Collision& collision);
}
}
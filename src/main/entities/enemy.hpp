#pragma once

#include "systems/constants.hpp"

namespace OneGunGame {
struct Renderable;
struct Collidable;
struct Collision;
struct Health;

namespace Entity {
    struct Setup;
}

namespace Enemy {
    entt::entity Create(Entity::Setup& setup);
    void Update(entt::registry& registry);

    struct Behavior {
        using Action = std::function<void(entt::registry &registry, entt::entity thisEntity)>;
        Action Act;

        Behavior(Action act) : Act(act) {}
    };
    
    Renderable& SetupRenderable(const Entity::Setup& setup, 
        Images imageID, const sf::IntRect& textureRect);
    Collidable& SetupCollidable(const Entity::Setup& setup, const sf::IntRect& collisionRect);
    Health& SetupHealth(const Entity::Setup& setup, float maxHealth, Callback callback = {});
    void SetupMovement(const Entity::Setup& setup, float moveSpeed);
    void SetupOffscreenLifetime(const Entity::Setup& setup, float expireTimeInSeconds);
    Behavior& SetupBehavior(const Entity::Setup& setup, Callback callback = {});
    void OnCollision(Collision& collision);
}
}
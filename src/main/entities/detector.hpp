#pragma once

#include "systems/constants.hpp"

namespace OneGunGame{

struct Collision;

namespace Entity {
    struct Setup;
}
struct Renderable;
struct Collidable;

namespace Detector {
    void Update(entt::registry& registry);
    entt::entity Create(Entity::Setup& setup);
    void OnCollision(Collision& collision);
    
    Renderable& SetupRenderable(const Entity::Setup& setup, 
        Images imageID, const sf::IntRect& textureRect);
    Collidable& SetupCollidable(const Entity::Setup& setup, const sf::IntRect& collisionRect);
    void SetupMovement(const Entity::Setup& setup, float moveSpeed);
}
}
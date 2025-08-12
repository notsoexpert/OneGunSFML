#pragma once

#include "systems/constants.hpp"
#include "entities/entity.hpp"

namespace OneGunGame{
struct Renderable;
struct Collidable;
struct Collision;

namespace Explosion {
    entt::entity Create(Entity::Setup& setup);
    Renderable& SetupRenderable(const Entity::Setup& setup, Images imageID, const sf::IntRect& textureRect);
    Collidable& SetupCollidable(const Entity::Setup& setup, const sf::IntRect& collisionRect);
    Entity::Setup SetupBasicExplosion(entt::registry& registry, const sf::Vector2f& position, const sf::Vector2f& direction);

    float GetExplosionDamage(entt::registry &registry, entt::entity explosionEntity, float baseDamage);
    void OnCollision(Collision& collision);
}
}
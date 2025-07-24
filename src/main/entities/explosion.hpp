#pragma once

#include "systems/constants.hpp"

struct Renderable;
struct Collidable;
struct Collision;

namespace Explosion {
    // Isn't this just a projectile? I don't know how to structure anything
    struct Setup {
        entt::registry& Registry;
        const sf::Vector2f& Position;
        const sf::Vector2f& Velocity;
        std::optional<OneGunGame::CollisionLayer> CollisionLayer;
        std::optional<uint8_t> CollisionMask;
        entt::entity Source = entt::null;
        entt::entity ThisEntity = entt::null;
    };

    entt::entity Create(Setup& setup);
    Renderable& SetupRenderable(const Setup& setup, OneGunGame::Images imageID, const sf::IntRect& textureRect);
    Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect);

    float GetExplosionDamage(entt::registry &registry, entt::entity explosionEntity, float baseDamage);
    void OnCollision(Collision& collision);
}
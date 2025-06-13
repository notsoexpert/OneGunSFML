#pragma once

#include "system/constants.hpp"

struct Renderable;
struct Collision;

namespace Explosion {
    // Isn't this just a projectile without Destructing? I don't know how to structure anything
    struct Setup {
        entt::registry &Registry;
        const sf::Vector2f &Position;
        const sf::Vector2f &Velocity;
        entt::entity Source = entt::null;
        entt::entity ThisEntity = entt::null;
    };

    entt::entity Create(Setup& setup);
    Renderable& SetupRenderable(const Setup& setup, OneGunGame::Images imageID, const sf::IntRect& textureRect);

    float GetExplosionDamage(entt::registry &registry, entt::entity explosionEntity, float baseDamage);
    void OnCollision(Collision& collision);
}
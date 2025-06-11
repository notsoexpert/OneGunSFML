#pragma once

#include "system/constants.hpp"

struct Renderable;
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
    float GetProjectileDamage(entt::registry &registry, entt::entity projectileEntity, float damage);

    Renderable& SetupRenderable(const Setup& setup, 
        OneGunGame::Images imageID, const sf::IntRect& textureRect);
    Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect);

    void OnCollision(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity);
}
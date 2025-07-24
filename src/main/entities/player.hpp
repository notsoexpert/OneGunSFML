#pragma once

#include "systems/constants.hpp"

namespace Projectile {
    struct Weapon;
}

struct Collision;

namespace Player {
    constexpr sf::Vector2i Size{64, 64};
    constexpr sf::Vector2i CollisionSize{32, 32}; // Collision size for the player sprite
    constexpr sf::Vector2i CollisionOffset{0, 0}; // Offset for collision detection

    constexpr sf::Vector2f Start(const sf::Vector2f &windowSize) {
        return {windowSize.x / 2.0f, windowSize.y - Size.y - 10.0f};
    }
    
    void Update(entt::registry& registry, entt::entity playerEntity);
    entt::entity Create(entt::registry &registry, const sf::Vector2f &startPosition);
    void Move(const sf::Vector2f &inputVector, entt::registry &registry, entt::entity playerEntity);
    void Dash(entt::registry &registry, entt::entity playerEntity);
    void OnCollision(Collision& collision);
    void OnDeath(entt::registry &registry, entt::entity playerEntity);
    void ChangeWeapon(Projectile::Weapon& weaponComponent);
}
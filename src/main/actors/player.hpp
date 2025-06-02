#pragma once

#include "system/constants.hpp"

namespace Player {
    constexpr sf::Vector2i Size{64, 64};
    constexpr sf::Vector2i CollisionSize{32, 32}; // Collision size for the player sprite
    constexpr sf::Vector2i CollisionOffset{-16, -16}; // Offset for collision detection

    constexpr sf::Vector2f Start(const sf::Vector2u &windowSize) {
        return {windowSize.x / 2.0f, windowSize.y - Size.y - 10.0f};
    }
    
    constexpr float BaseMaxHealth = 100.0f;
    constexpr float BaseMaxEnergy = 100.0f;
    constexpr float BaseMoveSpeed = 5.0f;
    constexpr float BaseAcceleration = 1.5f;
    constexpr float BaseFriction = 0.15f;
    constexpr float BaseHitInvincibilityDuration = 1.0f;
    constexpr float BaseDamage = 1.0f;
    constexpr float BaseFireRate = 1.0f;
    constexpr float BaseDashSpeedMultiplier = 3.0f;
    constexpr float BaseDashDuration = 0.25f;
    constexpr float BaseDashCooldown = 5.0f;

    void Update(entt::registry &registry, entt::entity playerEntity);
    entt::entity Create(entt::registry &registry, const sf::Vector2f &startPosition);
    void Move(const sf::Vector2f &inputVector, entt::registry &registry, entt::entity playerEntity);
    entt::entity Fire(entt::registry &registry, entt::entity playerEntity);
    void Dash(entt::registry &registry, entt::entity playerEntity);
    void OnCollision(entt::registry &registry, entt::entity playerEntity, entt::entity otherEntity);
}
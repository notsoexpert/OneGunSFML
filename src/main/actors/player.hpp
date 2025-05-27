#pragma once

#include "system/onegungame.hpp"

namespace Player {
    constexpr sf::Vector2i Size{64, 64};
    constexpr sf::Vector2f Start{OneGunGame::DefaultWindowSize.x / 2.0f, static_cast<float>(OneGunGame::DefaultWindowSize.y)};
    constexpr float DefaultMoveSpeed = 5.0f;

    struct Data {
        sf::Clock m_FireClock; // Clock to manage fire rate
        sf::Clock m_DashClock; // Clock to manage dash duration
        sf::Clock m_InvincibilityClock; // Clock to manage invincibility duration
        sf::Vector2f m_LastDirection{}; // Last direction the player moved in

        float m_MaxHealth = 100.0f; // Player's health
        float m_CurrentHealth = 100.0f; // Player's current health
        float m_MaxEnergy = 100.0f; // Player's energy
        float m_CurrentEnergy = 100.0f; // Player's current energy

        float m_BaseDamage = 1.0f; // Base damage dealt by the player
        float m_FireRate = 0.5f; // Player's fire rate

        float m_MoveAcceleration = 0.5f; // Player's movement acceleration
        float m_MoveSpeed = DefaultMoveSpeed; // Player's movement speed

        float m_DashSpeed = 10.0f; // Player's dash speed
        float m_DashDuration = 0.1f; // Duration of the dash in seconds
        float m_DashCooldown = 1.0f; // Cooldown time in seconds for the dash ability

        float m_InvincibilityTime = 2.0f; // Time in seconds for invincibility after being hit
        bool m_IsInvincible = false; // Flag to check if player is invincible
        bool m_CanDash = true; // Flag to check if player can dash
    };
}
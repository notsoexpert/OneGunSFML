#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

class Player {
public:
    Player() = default;
    Player(const Player&) = default;
    Player(Player&&) noexcept = default;
    Player& operator=(const Player&) = default;
    Player& operator=(Player&&) noexcept = default;
    ~Player() = default;

    // Add player-specific methods and attributes here
    void move(int direction);
    void dash();
    void fire();

private:
    sf::Sprite m_Sprite; // Player's sprite
    sf::Clock m_FireClock; // Clock to manage fire rate
    sf::Clock m_DashClock; // Clock to manage dash duration
    sf::Clock m_InvincibilityClock; // Clock to manage invincibility duration

    float m_MaxHealth = 100.0f; // Player's health
    float m_CurrentHealth = 100.0f; // Player's current health
    float m_MaxEnergy = 100.0f; // Player's energy
    float m_CurrentEnergy = 100.0f; // Player's current energy

    float m_BaseDamage = 1.0f; // Base damage dealt by the player
    float m_FireRate = 0.5f; // Player's fire rate

    float m_MoveAcceleration = 0.5f; // Player's movement acceleration
    float m_MoveSpeed = 5.0f; // Player's movement speed

    float m_DashSpeed = 10.0f; // Player's dash speed
    float m_DashDuration = 0.1f; // Duration of the dash in seconds
    float m_DashCooldown = 1.0f; // Cooldown time in seconds for the dash ability
    float m_DashDistance = 50.0f; // Distance covered during a dash

    float m_InvincibilityTime = 2.0f; // Time in seconds for invincibility after being hit
    bool m_IsInvincible = false; // Flag to check if player is invincible
    bool m_CanDash = true; // Flag to check if player can dash

    int m_LastDirection = 0; // Last direction the player moved in
};
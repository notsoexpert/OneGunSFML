#include "pch.hpp"
#include "player.hpp"

// Player::Player(const sf::Texture& texture, const sf::Vector2f& startPosition)
//     : m_Sprite(texture) 
// {
//     m_Sprite.setOrigin({32.0f, 32.0f}); // Set origin to center of the sprite   
//     m_Sprite.setPosition(startPosition);
//     m_Sprite.setTextureRect(sf::IntRect({0, 0}, Player::Size));

// }

// void Player::move(const sf::Vector2f &inputVector) {
//     // Update the player's position based on the direction
//     m_Sprite.move(inputVector * m_MoveSpeed);
//     m_LastDirection = inputVector; // Update last direction
// }

// void Player::dash() {
//     if (m_CanDash) {
//         // Perform the dash
//         m_Sprite.move(m_LastDirection * m_DashSpeed);
//         m_CanDash = false; // Set dash as used
//         m_DashClock.restart(); // Restart the dash clock
//     }
// }

// void Player::fire() {
//     if (m_FireClock.getElapsedTime().asSeconds() >= m_FireRate) {
//         // Fire logic here
//         m_FireClock.restart(); // Restart the fire clock
//     }
// }
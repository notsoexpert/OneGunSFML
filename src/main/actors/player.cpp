#include "pch.hpp"
#include "player.hpp"

#include "system/onegungame.hpp"
#include "components/components.hpp"
#include "actors/projectile.hpp"

namespace Player {
    entt::entity Fire(entt::registry &registry, entt::entity playerEntity, const sf::Texture& projectileTexture) {
        auto &playerData = registry.get<Player::Data>(playerEntity);
        if (playerData.m_FireClock.getElapsedTime().asSeconds() >= playerData.m_FireRate) {
            playerData.m_FireClock.restart();
            spdlog::trace("Player firing projectile");
            auto projectileType = static_cast<Projectile::Type>(OneGunGame::GetRandomGenerator().generateInt(0, static_cast<int>(Projectile::Type::Total) - 1));
            spdlog::trace("Projectile type: {}", static_cast<int>(projectileType));
            auto projectile = Projectile::Create(registry, projectileType, projectileTexture, registry.get<Renderable>(playerEntity).Sprite.getPosition(), sf::Vector2f{0.0f, -1.0f}, playerEntity);
            return projectile;
        }
        return entt::null;
    }

    entt::entity Create(entt::registry &registry, const sf::Texture &texture, const sf::Vector2f &startPosition) {
        entt::entity entity = registry.create();
        auto &renderable = registry.emplace<Renderable>(entity, texture, 10);
        renderable.Sprite.setOrigin({Player::Size.x / 2.0f, Player::Size.y / 2.0f}); // Set origin to center of the sprite
        renderable.Sprite.setPosition(startPosition);
        renderable.Sprite.setTextureRect(sf::IntRect({0, 0}, Player::Size));
        
        registry.emplace<Collidable>(entity, sf::IntRect{Player::CollisionOffset, Player::CollisionSize});
        registry.emplace<Velocity>(entity, 0.0f, 0.0f);
        registry.emplace<Player::Data>(entity);
        
        return entity;
    }
}
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
#pragma once

#include "system/constants.hpp"
#include "actors/player.hpp"
#include "actors/projectile.hpp"
#include "actors/enemy.hpp"

namespace OneGunGame {
    int Start(int argc, char *argv[]);
    std::expected<int, std::string> Setup();
    std::expected<int, std::string> Run();
    std::expected<int, std::string> Cleanup();
    
    void Update();
    void Render();

    entt::entity CreateBackground();
    entt::entity CreatePlayer();
    entt::entity CreateProjectile(Projectile::Type type, const sf::Vector2f& position, const sf::Vector2f& direction, entt::entity source);
    entt::entity CreateEnemy(Enemy::Type type, const sf::Vector2f& position, const sf::Vector2f& direction);

    sf::Vector2f GetInputVector();
    sf::Vector2u GetWindowSize();

}
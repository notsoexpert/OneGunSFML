#include "pch.hpp"
#include "entities/projectile_types.hpp"

namespace Projectile {
    static constexpr std::array<const char*, 2> Name = 
    {"Missile", "Homing Missile"};
    static constexpr std::array<OneGunGame::Images, 2> ImageID = 
    {OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet};
    static constexpr std::array<sf::IntRect, 2> TextureRect = 
    {sf::IntRect{{448, 0}, {64, 64}}, sf::IntRect{{512, 0}, {64, 64}}};
    static constexpr std::array<sf::IntRect, 2> CollisionRect = 
    {sf::IntRect{{0, 0}, {64, 64}}, sf::IntRect{{0, 0}, {64, 64}}};
    static constexpr std::array<float, 2> BaseDamage = 
    {4.0f, 4.0f};
    static constexpr std::array<float, 2> MoveSpeed = 
    {6.0f, 5.0f};
    static constexpr std::array<float, 2> OffscreenLifetime = 
    {1.0f, 1.0f};
    static constexpr std::array<size_t, 2> Specification = 
    {Flags::Destruct | Flags::Explode, Flags::Destruct | Flags::Explode | Flags::Home};

}
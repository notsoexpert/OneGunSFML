#include "pch.hpp"
#include "entities/projectile_types.hpp"

namespace Projectile {
    static constexpr std::array<const char*, 3> Name = 
    {"Green Laser", "Yellow Laser", "Red Laser"};
    static constexpr std::array<OneGunGame::Images, 3> ImageID = 
    {OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet};
    static constexpr std::array<sf::IntRect, 3> TextureRect = 
    {sf::IntRect{{256, 0}, {64, 64}}, sf::IntRect{{320, 0}, {64, 64}}, sf::IntRect{{384, 0}, {64, 64}}};
    static constexpr std::array<sf::IntRect, 3> CollisionRect = 
    {sf::IntRect{{-4, -8}, {8, 17}}, sf::IntRect{{-5, -13}, {10, 26}}, sf::IntRect{{-6, -18}, {12, 36}}};
    static constexpr std::array<float, 3> BaseDamage = 
    {2.0f, 2.0f, 2.0f};
    static constexpr std::array<float, 3> MoveSpeed = 
    {6.0f, 6.0f, 6.0f};
    static constexpr std::array<float, 3> OffscreenLifetime = 
    {1.0f, 1.0f, 1.0f};
    static constexpr std::array<size_t, 3> Specification = 
    {Flags::Destruct | Flags::Split, Flags::Destruct | Flags::Split, Flags::Destruct | Flags::Split};

}
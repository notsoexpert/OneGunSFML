#include "pch.hpp"
#include "entities/projectile_types.hpp"

namespace Projectile {
    static constexpr const char* Name = "Bomb";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionRed;
    static constexpr sf::IntRect TextureRect = {{576, 0}, {64, 64}};
    static constexpr float BaseDamage = 25.0f;
    static constexpr float MoveSpeed = 2.5f;
    static constexpr float OffscreenLifetime = 2.0f;
    static constexpr size_t Specification = Flags::Explode;
}
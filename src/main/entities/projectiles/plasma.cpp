#include "pch.hpp"
#include "entities/projectile_types.hpp"

namespace Projectile {
    static constexpr const char* Name = "Plasma";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionYellow;
    static constexpr sf::IntRect TextureRect = {{256, 0}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{-4, -8}, {8, 17}};
    static constexpr float BaseDamage = 3.0f;
    static constexpr float MoveSpeed = 4.0f;
    static constexpr float OffscreenLifetime = 1.5f;
    static constexpr size_t Specification = Flags::Burn;

}
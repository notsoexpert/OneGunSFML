#include "pch.hpp"
#include "entities/projectile_types.hpp"

namespace Projectile {
    static constexpr const char* Name = "Ice";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionBlue;
    static constexpr sf::IntRect TextureRect = {{256, 0}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{-4, -8}, {8, 17}};
    static constexpr float BaseDamage = 1.0f;
    static constexpr float MoveSpeed = 5.0f;
    static constexpr float OffscreenLifetime = 1.0f;
    static constexpr size_t Specification = Flags::Destruct;

}
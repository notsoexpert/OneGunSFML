#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"
#include "system/components.hpp"

namespace Projectile {
    static constexpr const char* Name = "Plasma";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionYellow;
    static constexpr sf::IntRect TextureRect = {{256, 0}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{-4, -8}, {8, 17}};
    static constexpr float BaseDamage = 3.0f;
    static constexpr float MoveSpeed = 4.0f;
    static constexpr float OffscreenLifetime = 1.5f;
    static constexpr size_t Specification = Flags::Burn;

    void PlasmaSetup(const Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed);

        setup.Registry.emplace<Component>(setup.ThisEntity, Plasma, 
            Specification, GetProjectileDamage(setup.Registry, setup.ThisEntity, BaseDamage));

        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);
    }
}
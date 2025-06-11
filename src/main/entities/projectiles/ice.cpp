#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"
#include "system/components.hpp"

namespace Projectile::Ice {
    static constexpr const char* Name = "Ice";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionBlue;
    static constexpr sf::IntRect TextureRect = {{256, 0}, {64, 64}};
    static constexpr sf::IntRect CollisionRect = {{-4, -8}, {8, 17}};
    static constexpr float BaseDamage = 1.0f;
    static constexpr float MoveSpeed = 5.0f;
    static constexpr float OffscreenLifetime = 1.0f;
    static constexpr size_t Specification = Flags::Destruct;

    void Create(const Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed);

        setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Ice, 
            Specification, GetProjectileDamage(setup.Registry, setup.ThisEntity, BaseDamage));

        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        auto &component = registry.get<Component>(thisEntity);

        if (component.CompareFlags(Flags::Explode)) {
            spdlog::info("Entity {} exploding!", static_cast<int>(thisEntity));
        }
    }
}
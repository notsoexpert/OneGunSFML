#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"
#include "system/components.hpp"

namespace Projectile::Bomb {
    static constexpr const char* Name = "Bomb";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionRed;
    static constexpr sf::IntRect TextureRect = {{576, 0}, {64, 64}};
    //static constexpr float BaseDamage = 25.0f;
    static constexpr float MoveSpeed = 2.5f;
    static constexpr float LifeTimeInSeconds = 2.0f;
    static constexpr size_t Specification = Flags::Explode;

    void Create(const Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        SetupRenderable(setup, ImageID, TextureRect);
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed);
        setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Bomb, Specification, 0.0f);
        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(LifeTimeInSeconds));
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        auto &component = registry.get<Component>(thisEntity);

        if (component.CompareFlags(Flags::Explode)) {
            spdlog::info("Entity {} exploding!", static_cast<int>(thisEntity));
        }
    }
}
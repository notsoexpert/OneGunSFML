#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"
#include "system/components.hpp"
#include "entities/explosion_types.hpp"

namespace Projectile::Ice {
    static constexpr const char* Name = "Ice";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionBlue;
    static constexpr sf::IntRect TextureRect = {{0, 32}, {32, 32}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {32, 32}};
    static constexpr float DefaultScale = 0.5f;
    static constexpr float BaseDamage = 1.0f;
    static constexpr float MoveSpeed = 6.0f;
    static constexpr float OffscreenLifetime = 1.0f;
    static constexpr size_t Specification = Flags::Destruct;

    void Create(const Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        auto& renderable = SetupRenderable(setup, ImageID, TextureRect);
        renderable.Sprite.setScale({DefaultScale, DefaultScale});
        SetupCollidable(setup, CollisionRect);
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed);

        setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Ice, 
            Specification, BaseDamage);

        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        registry.emplace<Destructing>(thisEntity);
        
        Explosion::Setup explosionSetup{
            registry,
            registry.get<Renderable>(thisEntity).Sprite.getPosition(),
            registry.get<Velocity>(thisEntity).Value,
            registry.get<Collidable>(thisEntity).Source
        };
        Explosion::IceHit::Create(explosionSetup);
    }
}
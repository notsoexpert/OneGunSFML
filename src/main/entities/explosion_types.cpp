#include "pch.hpp"
#include "explosion_types.hpp"

namespace Explosion {
    entt::entity Create(Setup& setup, Type type) 
    {
        if (type < Type::AsteroidDeath || type >= Type::Total) {
            spdlog::error("Create: Invalid explosion type: {}", static_cast<int>(type));
            return entt::null;
        }

        setup.ThisEntity = Explosion::Create(setup);

        registry.emplace<Component>(entity, type);

        

        registry.emplace<Velocity>(entity, velocity);

        OneGunGame::CollisionLayer mask = OneGunGame::GetHitMask(registry.get<Collidable>(source).Layer);

        registry.emplace<Collidable>(entity, specification.CollisionRect, source, 
            OneGunGame::CollisionLayer::Projectile, mask, OnCollision);
    }

    void OnCollision(entt::registry &registry, entt::entity explosionEntity, entt::entity otherEntity) {
        auto &component = registry.get<Component>(explosionEntity);

        float explosionDamage = GetExplosionDamage(registry, explosionEntity, component);

        Entity::Damage(registry, explosionEntity, otherEntity, explosionDamage);

        // TODO: Add damaged entity to ignore list
    }
}
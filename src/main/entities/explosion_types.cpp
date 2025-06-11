#include "pch.hpp"
#include "explosion_types.hpp"

#include "system/onegungame.hpp"
#include "system/components.hpp"
#include "entities/entity.hpp"

namespace Explosion {
    entt::entity Create(Setup& setup, Type type) 
    {
        if (type < Type::VisualOnly || type >= Type::Total) {
            spdlog::error("Create: Invalid explosion type: {}", static_cast<int>(type));
            return entt::null;
        }

        setup.ThisEntity = Explosion::Create(setup);

        setup.Registry.emplace<Component>(setup.ThisEntity, type);
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Velocity);

        //OneGunGame::CollisionLayer mask = OneGunGame::GetHitMask(setup.Registry.get<Collidable>(setup.Source).Layer);
        //setup.Registry.emplace<Collidable>(setup.ThisEntity, Specifications.at(type).CollisionRect, setup.Source, 
        //    OneGunGame::CollisionLayer::Projectile, mask, OnCollision);
        
        return setup.ThisEntity;
    }

    void OnCollision(entt::registry &registry, entt::entity explosionEntity, entt::entity otherEntity) {
        //auto &component = registry.get<Component>(explosionEntity);

        float explosionDamage = GetExplosionDamage(registry, explosionEntity, 25.0f);

        Entity::Damage(registry, explosionEntity, otherEntity, explosionDamage);

        // TODO: Add damaged entity to ignore list
    }
}
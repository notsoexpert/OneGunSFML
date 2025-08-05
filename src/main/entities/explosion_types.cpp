#include "pch.hpp"
#include "explosion_types.hpp"

#include "systems/onegungame.hpp"
#include "entities/entity.hpp"

#include "components/collidable.hpp"
#include "components/transformation.hpp"

namespace OneGunGame {
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

        //CollisionLayer mask = GetHitMask(setup.Registry.get<Collidable>(setup.Source).Layer);
        //setup.Registry.emplace<Collidable>(setup.ThisEntity, Specifications.at(type).CollisionRect, setup.Source, 
        //    CollisionLayer::Projectile, mask, OnCollision);
        
        return setup.ThisEntity;
    }

    void OnCollision(Collision& collision) {
        auto hitLimiting = collision.Registry.try_get<HitLimiting>(collision.ThisEntity);
        if (hitLimiting) {
            if (hitLimiting->HitEntities.contains(collision.OtherEntity)) {
                return;
            }
            hitLimiting->HitEntities.insert(collision.OtherEntity);
        }

        auto& component = collision.Registry.get<Component>(collision.ThisEntity);

        float explosionDamage = GetExplosionDamage(collision.Registry, collision.ThisEntity, component.BaseDamage);

        Entity::Damage(collision.Registry, collision.ThisEntity, collision.OtherEntity, explosionDamage);
    }
}
}
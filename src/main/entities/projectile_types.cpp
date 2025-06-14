#include "pch.hpp"
#include "projectile_types.hpp"

#include "system/components.hpp"
#include "system/onegungame.hpp"
#include "entities/entity.hpp"

#include "entities/explosion_types.hpp"

namespace Projectile {
    static const std::array<std::function<void(const Setup&)>, static_cast<size_t>(Type::Total)> ProjectileSetup = {
        Bullet::Create, Laser::Create, Plasma::Create,
        Missile::Create, Bomb::Create, Ice::Create
    };
    static const std::array<std::function<void(entt::registry&, entt::entity)>, static_cast<size_t>(Type::Total)> ProjectileDeath = {
        Bullet::Death, Laser::Death, Plasma::Death,
        Missile::Death, Bomb::Death, Ice::Death
    };

    entt::entity Create(Setup& setup, Type type) {
        if (type < Type::Bullet || type >= Type::Total) {
            spdlog::error("Create: Invalid projectile type: {}", static_cast<int>(type));
            return entt::null;
        }
        setup.ThisEntity = Projectile::Create(setup);

        ProjectileSetup.at(static_cast<size_t>(type))(setup);

        return setup.ThisEntity;
    }

    entt::entity Fire(entt::registry &registry, entt::entity thisEntity) {
        auto fireComponent = registry.try_get<Fireable>(thisEntity);
        if (!fireComponent) {
            spdlog::warn("Projectile::Fire - Entity {} does not have a Fireable component", static_cast<int>(thisEntity));
            return entt::null;
        }

        if (!fireComponent->Fire()) {
            spdlog::trace("Entity {} not ready to fire", static_cast<int>(thisEntity));
            return entt::null;
        }

        auto weaponComponent = registry.try_get<Projectile::Weapon>(thisEntity);
        if (!weaponComponent) {
            spdlog::warn("Projectile::Fire - Entity {} does not have a Weapon component", static_cast<int>(thisEntity));
            return entt::null;
        }

        spdlog::trace("Entity {} firing projectile", static_cast<int>(thisEntity));
        auto projectileType = weaponComponent->ProjectileType;
        auto fireDirection = weaponComponent->Direction.value_or(sf::Vector2f{0.0f, 1.0f});
        spdlog::trace("Projectile type: {}", static_cast<int>(projectileType));
        Projectile::Setup setup{registry, registry.get<Renderable>(thisEntity).Sprite.getPosition(), 
            fireDirection, thisEntity};
        return Projectile::Create(setup, projectileType);
    }

    void BurnEntity(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity, float damage, Burning& burningComponent) {
        if (!burningComponent.CanBurn(otherEntity)) {
            return;
        }
        
        if (Entity::Damage(registry, projectileEntity, otherEntity, damage)){
            burningComponent.BurnClocks[otherEntity].restart();
        }

        Explosion::Setup explosionSetup{
            registry,
            (registry.get<Renderable>(projectileEntity).Sprite.getPosition() * 0.5f) + (registry.get<Renderable>(otherEntity).Sprite.getPosition() * 0.5f),
            registry.get<Velocity>(projectileEntity).Value,
            registry.get<Collidable>(projectileEntity).Source
        };
        Explosion::VisualOnly::BurnHit::Create(explosionSetup);

        spdlog::info("Entity {} burned by entity {}", static_cast<int>(otherEntity), static_cast<int>(projectileEntity));
    }

    void OnCollision(Collision& collision) {
        auto hitLimiting = collision.Registry.try_get<HitLimiting>(collision.ThisEntity);
        if (hitLimiting) {
            if (hitLimiting->HitEntities.contains(collision.OtherEntity)) {
                return;
            }
            hitLimiting->HitEntities[collision.OtherEntity] = true;
        }

        auto &component = collision.Registry.get<Component>(collision.ThisEntity);
        float projectileDamage = GetProjectileDamage(collision.Registry, collision.ThisEntity, component.BaseDamage);

        if (component.CompareFlags(Flags::Burn)) {
            auto &burning = collision.Registry.get<Burning>(collision.ThisEntity);
            BurnEntity(collision.Registry, collision.ThisEntity, collision.OtherEntity, projectileDamage, burning);
        } else {
            Entity::Damage(collision.Registry, collision.ThisEntity, collision.OtherEntity, projectileDamage);
        }

        if (component.CompareFlags(Flags::Destruct)) {
            collision.Registry.emplace_or_replace<Dying>(collision.ThisEntity, ProjectileDeath.at(static_cast<size_t>(component.ThisType)));
        }
        
    }
}
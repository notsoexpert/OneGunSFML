#include "pch.hpp"
#include "projectile_types.hpp"

#include "system/components.hpp"
#include "system/onegungame.hpp"
#include "entities/entity.hpp"

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

    void BurnEntity(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity, float damage, Burning& burningComponent) {
        if (!burningComponent.CanBurn(otherEntity)) {
            return;
        }
        
        if (Entity::Damage(registry, projectileEntity, otherEntity, damage)){
            burningComponent.BurnClocks[otherEntity].restart();
        }

        spdlog::info("Entity {} burned by entity {}", static_cast<int>(otherEntity), static_cast<int>(projectileEntity));
    }

    void OnCollision(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity) {
        auto hitLimiting = registry.try_get<HitLimiting>(projectileEntity);
        if (hitLimiting) {
            if (hitLimiting->HitEntities.contains(otherEntity)) {
                return;
            }
            hitLimiting->HitEntities[otherEntity] = true;
        }

        auto &component = registry.get<Component>(projectileEntity);
        float projectileDamage = GetProjectileDamage(registry, projectileEntity, component.BaseDamage);

        if (component.CompareFlags(Flags::Burn)) {
            auto &burning = registry.get<Burning>(projectileEntity);
            BurnEntity(registry, projectileEntity, otherEntity, projectileDamage, burning);
        } else {
            Entity::Damage(registry, projectileEntity, otherEntity, projectileDamage);
        }

        if (component.CompareFlags(Flags::Destruct)) {
            registry.emplace_or_replace<Dying>(projectileEntity, ProjectileDeath.at(static_cast<size_t>(component.ThisType)));
        }
        
    }
}
#include "pch.hpp"
#include "projectile_types.hpp"

#include "systems/components.hpp"
#include "systems/onegungame.hpp"
#include "entities/entity.hpp"
#include "entities/explosion_types.hpp"
#include "entities/enemy_types.hpp"
#include "entities/weapon.hpp"

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

    entt::entity Fire(entt::registry &registry, entt::entity thisEntity, std::optional<uint8_t> tierOverride) {
        auto fireComponent = registry.try_get<Fireable>(thisEntity);
        if (!fireComponent) {
            spdlog::warn("Projectile::Fire - Entity {} does not have a Fireable component", static_cast<int>(thisEntity));
            return entt::null;
        }

        auto weaponComponent = registry.try_get<Weapon::Component>(thisEntity);
        if (!weaponComponent) {
            spdlog::warn("Projectile::Fire - Entity {} does not have a Weapon component", static_cast<int>(thisEntity));
            return entt::null;
        }
        
        if (!fireComponent->Fire(weaponComponent->GetFireRate())) {
            spdlog::trace("Entity {} not ready to fire", static_cast<int>(thisEntity));
            return entt::null;
        }
        
        auto fireDirection = weaponComponent->ForwardVector;

        auto renderableComponent = registry.try_get<Renderable>(thisEntity);
        if (renderableComponent) {
            fireDirection = fireDirection.rotatedBy(renderableComponent->Sprite.getRotation());
        }

        auto projectileType = weaponComponent->ProjectileType;
        auto collisionLayer = OneGunGame::CollisionLayer::NeutralProjectile;
        auto collisionMask = OneGunGame::CollisionLayer::Player | OneGunGame::CollisionLayer::Enemy | OneGunGame::CollisionLayer::Obstacle;
        if (thisEntity == OneGunGame::GetPlayerEntity()) {
            collisionLayer = OneGunGame::CollisionLayer::PlayerProjectile;
            collisionMask &= ~OneGunGame::CollisionLayer::Player;
        } else if (registry.try_get<Enemy::Component>(thisEntity)) {
            collisionLayer = OneGunGame::CollisionLayer::EnemyProjectile;
            collisionMask &= ~OneGunGame::CollisionLayer::Enemy;
        }
        
        Projectile::Setup setup{
            registry, 
            registry.get<Renderable>(thisEntity).Sprite.getPosition(), 
            fireDirection,
            collisionLayer,
            static_cast<uint8_t>(collisionMask),
            thisEntity, 
            entt::null, 
            tierOverride.value_or(weaponComponent->ProjectileTier)
        };
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
            registry.get<Velocity>(projectileEntity).Value
        };
        Explosion::BurnHit::Create(explosionSetup);

        spdlog::info("Entity {} burned by entity {}", static_cast<int>(otherEntity), static_cast<int>(projectileEntity));
    }

    void OnCollision(Collision& collision) {
        auto hitLimiting = collision.Registry.try_get<HitLimiting>(collision.ThisEntity);
        if (hitLimiting) {
            if (hitLimiting->HitEntities.contains(collision.OtherEntity)) {
                return;
            }
            hitLimiting->HitEntities.insert(collision.OtherEntity);
        }
        
        auto &component = collision.Registry.get<Component>(collision.ThisEntity);
        float projectileDamage = GetProjectileDamage(collision.Registry, collision.ThisEntity, component.DamageFactor);

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
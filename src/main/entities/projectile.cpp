#include "pch.hpp"
#include "projectile.hpp"

#include "entities/entity.hpp"
#include "system/onegungame.hpp"
#include "system/components.hpp"

#include "entities/player.hpp"

namespace Projectile {

    entt::entity Create(entt::registry &registry, 
        Type type, const sf::Vector2f& position, const sf::Vector2f& direction, 
        entt::entity source)
    {
        if (type < Type::Bullet1 || type >= Type::Total) {
            spdlog::error("Create: Invalid projectile type: {}", static_cast<int>(type));
            return entt::null;
        }
        auto &specification = Specifications.at(static_cast<size_t>(type));
        
        entt::entity entity = registry.create();

        registry.emplace<Component>(entity, type);

        auto &projectileSprite = registry.emplace<Renderable>(entity, 
            OneGunGame::GetTexture(OneGunGame::SpriteSheet), 25).Sprite;
        projectileSprite.setTextureRect(specification.TextureRect);
        projectileSprite.setOrigin({specification.TextureRect.size.x / 2.0f, specification.TextureRect.size.y / 2.0f});
        projectileSprite.setPosition(position);

        registry.emplace<Velocity>(entity, direction * specification.Speed);

        OneGunGame::CollisionLayer mask = OneGunGame::GetHitMask(registry.get<Collidable>(source).Layer);
        
        registry.emplace<Collidable>(entity, specification.CollisionRect, source, 
            OneGunGame::CollisionLayer::Projectile, mask, OnCollision);

        auto &lifetime = registry.emplace<Lifetime>(entity, sf::seconds(specification.Lifetime));
        lifetime.Clock.restart();

        return entity;
    }

    float GetProjectileDamage(entt::registry &registry, entt::entity projectileEntity, const Component &component) {
        float damage = Specifications.at(component.ThisType).Damage;

        entt::entity sourceEntity = registry.get<Collidable>(projectileEntity).Source;
        if (!registry.valid(sourceEntity)) {
            spdlog::warn("Projectile entity {} - Source entity {} is invalid", static_cast<int>(projectileEntity), static_cast<int>(sourceEntity));
            return damage;
        }
        
        auto sourceFireable = registry.try_get<Fireable>(sourceEntity);
        if (!sourceFireable) {
            spdlog::warn("Projectile entity {} - Source entity {} is missing Fireable component", static_cast<int>(projectileEntity), static_cast<int>(sourceEntity));
            return damage;
        }

        return damage * sourceFireable->BaseDamage;
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
        auto &component = registry.get<Component>(projectileEntity);

        float projectileDamage = GetProjectileDamage(registry, projectileEntity, component);

        if (component.CompareFlags(Specification::Burning)) {
            auto &burning = registry.get<Burning>(projectileEntity);
            BurnEntity(registry, projectileEntity, otherEntity, projectileDamage, burning);
        } else if (!component.CompareFlags(Specification::Exploding)) {
            Entity::Damage(registry, projectileEntity, otherEntity, projectileDamage);
        }
        
        // TODO: Create explosion actor and generate here
        if (component.CompareFlags(Specification::Exploding)) {
            spdlog::info("Projectile {} exploded on collision with entity {}", static_cast<int>(projectileEntity), static_cast<int>(otherEntity));
        }
        
        if (component.CompareFlags(Specification::Destructing)) {
            registry.emplace_or_replace<Destructing>(projectileEntity);
        }
        
    }


}
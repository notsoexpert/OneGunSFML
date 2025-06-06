#include "pch.hpp"
#include "explosion.hpp"

#include "entities/entity.hpp"
#include "system/onegungame.hpp"
#include "system/components.hpp"

namespace Explosion {

    entt::entity Create(entt::registry &registry, Type type, 
        const sf::Vector2f position, const sf::Vector2f velocity, entt::entity source) 
    {
        if (type < Type::AsteroidDeath || type >= Type::Total) {
            spdlog::error("Create: Invalid explosion type: {}", static_cast<int>(type));
            return entt::null;
        }
        auto &specification = Specifications.at(static_cast<size_t>(type));
        
        entt::entity entity = registry.create();

        registry.emplace<Component>(entity, type);

        auto &explosionSprite = registry.emplace<Renderable>(entity, 
            OneGunGame::GetTexture(specification.TextureID), 20).Sprite;
        explosionSprite.setTextureRect(specification.TextureRect);
        explosionSprite.setOrigin({specification.TextureRect.size.x / 2.0f, specification.TextureRect.size.y / 2.0f});
        explosionSprite.setPosition(position);

        registry.emplace<Velocity>(entity, velocity);

        OneGunGame::CollisionLayer mask = OneGunGame::GetHitMask(registry.get<Collidable>(source).Layer);

        registry.emplace<Collidable>(entity, specification.CollisionRect, source, 
            OneGunGame::CollisionLayer::Projectile, mask, OnCollision);

        return entity;

    }

    void OnCollision(entt::registry &registry, entt::entity explosionEntity, entt::entity otherEntity) {
        auto &component = registry.get<Component>(explosionEntity);

        float explosionDamage = GetExplosionDamage(registry, explosionEntity, component);

        Entity::Damage(registry, explosionEntity, otherEntity, explosionDamage);

        // TODO: Add damaged entity to ignore list
    }

    float GetExplosionDamage(entt::registry &registry, entt::entity explosionEntity, const Component &component) {
        float damage = Specifications.at(component.ThisType).Damage;

        entt::entity sourceEntity = registry.get<Collidable>(explosionEntity).Source;
        if (!registry.valid(sourceEntity)) {
            spdlog::warn("Explosion entity {} - Source entity {} is invalid", static_cast<int>(explosionEntity), static_cast<int>(sourceEntity));
            return damage;
        }
        
        auto sourceFireable = registry.try_get<Fireable>(sourceEntity);
        if (!sourceFireable) {
            spdlog::warn("Explosion entity {} - Source entity {} is missing Fireable component", static_cast<int>(explosionEntity), static_cast<int>(sourceEntity));
            return damage;
        }

        return damage * sourceFireable->BaseDamage;
    }
}
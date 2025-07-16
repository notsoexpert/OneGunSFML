#include "pch.hpp"
#include "explosion.hpp"

#include "entities/entity.hpp"
#include "system/onegungame.hpp"
#include "system/components.hpp"

#include "entities/projectile_types.hpp"
#include "entities/weapon.hpp"

namespace Explosion {

    entt::entity Create(Setup& setup) {   
        entt::entity entity = setup.Registry.create();

        return entity;
    }

    Renderable& SetupRenderable(const Setup& setup, OneGunGame::Images imageID, const sf::IntRect& textureRect) {
        auto& renderable = setup.Registry.emplace<Renderable>(setup.ThisEntity, 
            OneGunGame::GetTexture(imageID), 20);
        renderable.Sprite.setTextureRect(textureRect);
        renderable.Sprite.setOrigin({textureRect.size.x / 2.0f, textureRect.size.y / 2.0f});
        renderable.Sprite.setPosition(setup.Position);
        return renderable;
    }

    Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect){
        sf::IntRect centeredRect = collisionRect;
        centeredRect.position -= centeredRect.size / 2;
        return setup.Registry.emplace<Collidable>(
            setup.ThisEntity, 
            collisionRect, 
            setup.Source, 
            setup.CollisionLayer.value_or(
                OneGunGame::CollisionLayer::NeutralProjectile
            ), 
            setup.CollisionMask.value_or(
                static_cast<uint8_t>(OneGunGame::CollisionLayer::Obstacle)
            ),
            OnCollision
        );
    }

    float GetExplosionDamage(entt::registry &registry, entt::entity explosionEntity, float damageFactor) {
        entt::entity sourceEntity = registry.get<Collidable>(explosionEntity).Source;
        if (!registry.valid(sourceEntity)) {
            spdlog::warn("Explosion entity {} - Source entity {} is invalid", static_cast<int>(explosionEntity), static_cast<int>(sourceEntity));
            return damageFactor;
        }
        
        auto sourceWeapon = registry.try_get<Weapon::Component>(sourceEntity);
        if (!sourceWeapon) {
            spdlog::warn("Explosion entity {} - Source entity {} is missing Weapon component", static_cast<int>(explosionEntity), static_cast<int>(sourceEntity));
            return damageFactor;
        }

        return damageFactor * sourceWeapon->BaseDamage;
    }
}
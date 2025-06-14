#include "pch.hpp"
#include "projectile.hpp"

#include "entities/entity.hpp"
#include "system/onegungame.hpp"
#include "system/components.hpp"

#include "entities/player.hpp"

namespace Projectile {
    entt::entity Create(Setup& setup)
    {
        entt::entity entity = setup.Registry.create();

        return entity;
    }

    Renderable& SetupRenderable(const Setup& setup, OneGunGame::Images imageID, const sf::IntRect& textureRect) {
        auto &renderable = setup.Registry.emplace<Renderable>(setup.ThisEntity, 
            OneGunGame::GetTexture(imageID), 25);
        renderable.Sprite.setTextureRect(textureRect);
        renderable.Sprite.setOrigin({textureRect.size.x / 2.0f, textureRect.size.y / 2.0f});
        renderable.Sprite.setPosition(setup.Position);
        return renderable;
    }
    Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect){
        sf::IntRect centeredRect = collisionRect;
        centeredRect.position -= centeredRect.size / 2;
        OneGunGame::CollisionLayer mask = OneGunGame::GetHitMask(OneGunGame::CollisionLayer::Projectile);
        return setup.Registry.emplace<Collidable>(setup.ThisEntity, collisionRect, 
            setup.Source, OneGunGame::CollisionLayer::Projectile, mask,
            OnCollision);
    }

    float GetProjectileDamage(entt::registry &registry, entt::entity projectileEntity, float baseDamage) {
        float basePower = Entity::GetBasePower(registry, registry.get<Collidable>(projectileEntity).Source);
        return baseDamage * basePower;
    }
}
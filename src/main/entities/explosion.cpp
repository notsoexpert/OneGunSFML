#include "pch.hpp"
#include "explosion.hpp"

#include "entities/entity.hpp"
#include "system/onegungame.hpp"
#include "system/components.hpp"

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

    float GetExplosionDamage(entt::registry &registry, entt::entity explosionEntity, float baseDamage) {
        entt::entity sourceEntity = registry.get<Collidable>(explosionEntity).Source;
        if (!registry.valid(sourceEntity)) {
            spdlog::warn("Explosion entity {} - Source entity {} is invalid", static_cast<int>(explosionEntity), static_cast<int>(sourceEntity));
            return baseDamage;
        }
        
        auto sourceFireable = registry.try_get<Fireable>(sourceEntity);
        if (!sourceFireable) {
            spdlog::warn("Explosion entity {} - Source entity {} is missing Fireable component", static_cast<int>(explosionEntity), static_cast<int>(sourceEntity));
            return baseDamage;
        }

        return baseDamage * sourceFireable->BaseDamage;
    }
}
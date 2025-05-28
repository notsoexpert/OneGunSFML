#include "pch.hpp"
#include "projectile.hpp"

#include "system/onegungame.hpp"
#include "components/components.hpp"

namespace Projectile {

    entt::entity Create(entt::registry &registry, 
        Type type, const sf::Texture &texture, 
        const sf::Vector2f& position, const sf::Vector2f& direction, 
        entt::entity source)
    {
        entt::entity entity = registry.create();
        auto &projectileSprite = registry.emplace<Renderable>(entity, texture, 25).Sprite;
        auto &preset = Presets.at(static_cast<size_t>(type));
        projectileSprite.setTextureRect(preset.TextureRect);
        projectileSprite.setOrigin({preset.TextureRect.size.x / 2.0f, preset.TextureRect.size.y / 2.0f});
        projectileSprite.setPosition(position);
        registry.emplace<Velocity>(entity, direction * preset.Speed);
        registry.emplace<Collidable>(entity, preset.CollisionRect, source);
        auto &lifetime = registry.emplace<Lifetime>(entity, sf::seconds(preset.Lifetime));
        lifetime.Clock.restart();
        return entity;
    }
}
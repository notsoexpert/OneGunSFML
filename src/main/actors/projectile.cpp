#include "pch.hpp"
#include "projectile.hpp"

#include "system/onegungame.hpp"
#include "components/components.hpp"

#include "actors/player.hpp"

namespace Projectile {

    entt::entity Create(entt::registry &registry, 
        Type type, const sf::Texture &texture, 
        const sf::Vector2f& position, const sf::Vector2f& direction, 
        entt::entity source)
    {
        if (type < Type::Bullet1 || type >= Type::Total) {
            spdlog::error("Invalid projectile type: {}", static_cast<int>(type));
            return entt::null;
        }
        
        entt::entity entity = registry.create();

        registry.emplace<Component>(entity, type);

        auto &projectileSprite = registry.emplace<Renderable>(entity, texture, 25).Sprite;
        auto &preset = Presets.at(static_cast<size_t>(type));
        projectileSprite.setTextureRect(preset.TextureRect);
        projectileSprite.setOrigin({preset.TextureRect.size.x / 2.0f, preset.TextureRect.size.y / 2.0f});
        projectileSprite.setPosition(position);

        registry.emplace<Velocity>(entity, direction * preset.Speed);

        OneGunGame::CollisionLayer mask;
        switch (registry.get<Collidable>(source).Layer) {
            case OneGunGame::CollisionLayer::Player:
                mask = static_cast<OneGunGame::CollisionLayer>
                (OneGunGame::CollisionLayer::Enemy | OneGunGame::CollisionLayer::Obstacle);
                break;
            case OneGunGame::CollisionLayer::Projectile:
                mask = static_cast<OneGunGame::CollisionLayer>
                (OneGunGame::CollisionLayer::Player | OneGunGame::CollisionLayer::Enemy | 
                    OneGunGame::CollisionLayer::Obstacle);
                break;
            case OneGunGame::CollisionLayer::Enemy:
                mask = static_cast<OneGunGame::CollisionLayer>
                (OneGunGame::CollisionLayer::Player | OneGunGame::CollisionLayer::Obstacle);
                break;
            case OneGunGame::CollisionLayer::Obstacle:
                mask = static_cast<OneGunGame::CollisionLayer>
                (OneGunGame::CollisionLayer::Player | OneGunGame::CollisionLayer::Projectile | 
                    OneGunGame::CollisionLayer::Enemy);
                break;
        }
        auto colRect = preset.CollisionRect;
        registry.emplace<Collidable>(entity, preset.CollisionRect, source, 
            OneGunGame::CollisionLayer::Projectile, mask, OnCollision);

        auto &lifetime = registry.emplace<Lifetime>(entity, sf::seconds(preset.Lifetime));
        lifetime.Clock.restart();

        return entity;
    }

    void OnCollision(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity) {
        auto &component = registry.get<Component>(projectileEntity);
        entt::entity sourceEntity = registry.get<Collidable>(projectileEntity).Source;
        float damage = Presets.at(component.ThisType).Damage;

        if (registry.valid(sourceEntity)) {
            const auto &sourceFireable = registry.get<Fireable>(sourceEntity);
            damage *= sourceFireable.BaseDamage;
        }
        
        auto otherHealth = registry.try_get<Health>(otherEntity);
        if (!otherHealth) {
            spdlog::warn("Entity {} hit by projectile entity {} is missing health component", 
            static_cast<int>(otherEntity), static_cast<int>(projectileEntity));
        }
        if (component.CompareFlags(Config::Burning)) {
            auto &burning = registry.get<Burning>(projectileEntity);
            if (burning.CanBurn(otherEntity)) {
                if (otherHealth) {
                    otherHealth->Damage(damage);
                }
                burning.BurnClocks[otherEntity].restart();
            }
        } else {
            if (otherHealth) {
                spdlog::warn("Entity {} damaged. New Health: {}", static_cast<int>(otherEntity), otherHealth->Current);
                otherHealth->Damage(damage);
            }
        }
        
        // TODO: Create explosion actor and generate here
        if (component.CompareFlags(Config::Exploding)) {
            spdlog::info("Projectile {} exploded on collision with entity {}", static_cast<int>(projectileEntity), static_cast<int>(otherEntity));
        }
        
        if (component.CompareFlags(Config::Destructing)) {
            registry.emplace<Destructing>(projectileEntity);
        }
        
    }


}
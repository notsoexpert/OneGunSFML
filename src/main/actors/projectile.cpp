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
        registry.emplace<Collidable>(entity, preset.CollisionRect, source, 
            OneGunGame::CollisionLayer::Projectile, mask, OnCollision);

        auto &lifetime = registry.emplace<Lifetime>(entity, sf::seconds(preset.Lifetime));
        lifetime.Clock.restart();

        return entity;
    }

    float GetProjectileDamage(entt::registry &registry, entt::entity projectileEntity, const Component &component) {
        float damage = Presets.at(component.ThisType).Damage;

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

    void DamageEntity(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity, float damage) {
        auto otherHealth = registry.try_get<Health>(otherEntity);
        if (!otherHealth) {
            spdlog::error("Entity {} hit by projectile entity {} is missing health component", 
            static_cast<int>(otherEntity), static_cast<int>(projectileEntity));
            return;
        }
        
        spdlog::info("Entity {} damaged. New Health: {}", static_cast<int>(otherEntity), otherHealth->Current);
        otherHealth->Damage(damage);
    }

    void BurnEntity(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity, float damage, Burning& burningComponent) {
        auto otherHealth = registry.try_get<Health>(otherEntity);
        if (!otherHealth) {
            spdlog::error("Entity {} hit by projectile entity {} is missing health component", 
            static_cast<int>(otherEntity), static_cast<int>(projectileEntity));
            return;
        }

        if (!burningComponent.CanBurn(otherEntity)) {
            return;
        }
        
        otherHealth->Damage(damage);
        spdlog::info("Entity {} burned. New Health: {}", static_cast<int>(otherEntity), otherHealth->Current);
        burningComponent.BurnClocks[otherEntity].restart();

    }

    void OnCollision(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity) {
        auto &component = registry.get<Component>(projectileEntity);

        float projectileDamage = GetProjectileDamage(registry, projectileEntity, component);

        if (component.CompareFlags(Config::Burning)) {
            auto &burning = registry.get<Burning>(projectileEntity);
            BurnEntity(registry, projectileEntity, otherEntity, projectileDamage, burning);
        } else if (!component.CompareFlags(Config::Exploding)) {
            DamageEntity(registry, projectileEntity, otherEntity, projectileDamage);
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
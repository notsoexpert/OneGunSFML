#include "pch.hpp"
#include "enemy.hpp"

#include "system/onegungame.hpp"
#include "components/components.hpp"

#include "actors/projectile.hpp"

namespace Enemy {

    const std::unordered_map<Type, Behavior::Action> Behavior::Callbacks = {
            {Comet, CometBehavior},
            {Fighter, FighterBehavior},
            {Bomber, BomberBehavior},
            {Hunter, HunterBehavior}, 
            {Bombardier, BombardierBehavior},
            {Galaxis, GalaxisBehavior}
        };

    void Update(entt::registry &registry) {
        registry.view<Behavior>().each(
            [&](auto entity, Behavior &behavior) {
                behavior.Act(registry, entity);
            }
        );
    }

    entt::entity Create(entt::registry &registry, Type type,const sf::Vector2f& position, 
        const sf::Vector2f& direction, entt::entity source) {

        entt::entity entity = registry.create();

        auto &renderable = registry.emplace<Renderable>(entity, 
            OneGunGame::GetTexture(OneGunGame::SpriteSheet), 50);
        renderable.Sprite.setTextureRect(Presets.at(type).TextureRect);
        renderable.Sprite.setOrigin(static_cast<sf::Vector2f>(renderable.Sprite.getTextureRect().size) / 2.0f);
        renderable.Sprite.setPosition(position);
        
        registry.emplace<Velocity>(entity, direction * Presets.at(type).MoveSpeed);
        
        sf::IntRect centeredRect = Presets.at(type).CollisionRect;
        centeredRect.position -= centeredRect.size / 2;
        registry.emplace<Collidable>(entity, Presets.at(type).CollisionRect, source, OneGunGame::CollisionLayer::Enemy,
            static_cast<OneGunGame::CollisionLayer>(OneGunGame::Player | OneGunGame::Projectile),
            OnCollision);

        registry.emplace<Health>(entity, Presets.at(type).MaxHealth);
        registry.emplace<MaxSpeed>(entity);
        registry.emplace<Fireable>(entity, 1.0f, 5.0f);
        registry.emplace<Projectile::Weapon>(entity, Projectile::Weapon::Cannon);
        if (Behavior::Callbacks.contains(type))
            registry.emplace<Behavior>(entity, Behavior::Callbacks.at(type));
        registry.emplace<Component>(entity, type);
        registry.emplace<ScreenTrigger>(entity, RemoveOffscreenLifetime, AddOffscreenLifetime);
        registry.emplace<Rotating>(entity, sf::radians(2*OneGunGame::Pi));

        spdlog::info("Creating Enemy of type '{}', entity {}", Names.at(type), static_cast<int>(entity));
        return entity;
    }

    void OnCollision(entt::registry &registry, entt::entity thisEntity, entt::entity otherEntity) {
        if (registry.valid(thisEntity) && registry.valid(otherEntity)) {
            spdlog::trace("Enemy::OnCollision invoked between entity {} and entity {}", 
            static_cast<int>(thisEntity), static_cast<int>(otherEntity));
        }
    }

    void RemoveOffscreenLifetime(entt::registry &registry, entt::entity thisEntity) {
        if (registry.remove<Lifetime>(thisEntity) > 0)
            spdlog::trace("Lifetime component removed from {}", static_cast<int>(thisEntity));
    }

    void AddOffscreenLifetime(entt::registry &registry, entt::entity thisEntity) {
        auto component = registry.try_get<Component>(thisEntity);
        if (!component)
            return;

        auto lifetime = registry.try_get<Lifetime>(thisEntity);
        if (!lifetime) {
            registry.emplace<Lifetime>(thisEntity, sf::seconds(Presets.at(component->ThisType).OffscreenLifetime));
            spdlog::trace("Lifetime component added to {}", static_cast<int>(thisEntity));
        }
    }
    void CometBehavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity))
            spdlog::trace("Comet Behavior invoked for entity {}", static_cast<int>(thisEntity));
    }
    void FighterBehavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity))
            spdlog::trace("Fighter Behavior invoked for entity {}", static_cast<int>(thisEntity));
    }
    void BomberBehavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity))
            spdlog::trace("Bomber Behavior invoked for entity {}", static_cast<int>(thisEntity));
    }
    void HunterBehavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity))
            spdlog::trace("Hunter Behavior invoked for entity {}", static_cast<int>(thisEntity));
    }
    void BombardierBehavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity))
            spdlog::trace("Bombardier Behavior invoked for entity {}", static_cast<int>(thisEntity));
    }
    void GalaxisBehavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity))
            spdlog::trace("Galaxis Behavior invoked for entity {}", static_cast<int>(thisEntity));
    }

}
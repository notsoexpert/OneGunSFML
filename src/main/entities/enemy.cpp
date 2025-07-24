#include "pch.hpp"
#include "enemy.hpp"

#include "systems/onegungame.hpp"
#include "entities/entity.hpp"
#include "entities/projectile_types.hpp"

namespace Enemy{
void Update(entt::registry &registry) {
    registry.view<Behavior>().each(
        [&](auto entity, Behavior &behavior) {
            behavior.Act(registry, entity);
        }
    );
}
entt::entity Create(Setup& setup) {
    setup.ThisEntity = setup.Registry.create();
    
    return setup.ThisEntity;
}
Renderable& SetupRenderable(const Setup& setup, OneGunGame::Images imageID, const sf::IntRect& textureRect){
    auto &renderable = setup.Registry.emplace<Renderable>(setup.ThisEntity, 
        OneGunGame::GetTexture(imageID), 50);
    renderable.Sprite.setTextureRect(textureRect);
    renderable.Sprite.setOrigin(static_cast<sf::Vector2f>(renderable.Sprite.getTextureRect().size) / 2.0f);
    renderable.Sprite.setPosition(setup.Position);
    return renderable;
}
Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect){
    return setup.Registry.emplace<Collidable>(
        setup.ThisEntity, 
        collisionRect, 
        setup.Source, 
        setup.CollisionLayer.value_or(
            OneGunGame::CollisionLayer::Enemy
        ),
        setup.CollisionMask.value_or(
            static_cast<uint8_t>(
                OneGunGame::Player | OneGunGame::PlayerProjectile
            )
        ),
        OnCollision);
}
Health& SetupHealth(const Setup& setup, float maxHealth, Callback callback){
    return setup.Registry.emplace<Health>(setup.ThisEntity, maxHealth, callback);
}
Behavior& SetupBehavior(const Setup& setup, Callback callback){
    return setup.Registry.emplace<Behavior>(setup.ThisEntity, callback);
}
void SetupMovement(const Setup& setup, float moveSpeed){
    setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * moveSpeed);
    setup.Registry.emplace<MaxSpeed>(setup.ThisEntity, moveSpeed);
}
void SetupOffscreenLifetime(const Setup& setup, float expireTimeInSeconds){
    setup.Registry.emplace<ScreenTrigger>(setup.ThisEntity, Entity::RemoveOffscreenLifetime, Entity::AddOffscreenLifetime, expireTimeInSeconds);
}
void OnCollision(Collision& collision) {
    if (collision.Registry.valid(collision.ThisEntity) && collision.Registry.valid(collision.OtherEntity)) {
        spdlog::trace("Enemy::OnCollision invoked between entity {} and entity {}", 
        static_cast<int>(collision.ThisEntity), static_cast<int>(collision.OtherEntity));
    }
}
}
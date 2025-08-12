#include "pch.hpp"
#include "enemy.hpp"

#include "systems/onegungame.hpp"
#include "entities/entity.hpp"
#include "entities/projectile_types.hpp"

#include "components/renderable.hpp"
#include "components/collidable.hpp"
#include "components/lifetime.hpp"
#include "components/transformation.hpp"

namespace OneGunGame {
namespace Enemy{
void Update(entt::registry &registry) {
    registry.view<Behavior>().each(
        [&](auto entity, Behavior &behavior) {
            behavior.Act(registry, entity);
        }
    );
}
entt::entity Create(Entity::Setup& setup) {
    setup.ThisEntity = setup.Registry.create();
    
    return setup.ThisEntity;
}
Renderable& SetupRenderable(const Entity::Setup& setup, Images imageID, const sf::IntRect& textureRect){
    auto &renderable = setup.Registry.emplace<Renderable>(setup.ThisEntity, 
        GetTexture(imageID), 50);
    renderable.Sprite.setTextureRect(textureRect);
    renderable.Sprite.setOrigin(static_cast<sf::Vector2f>(renderable.Sprite.getTextureRect().size) / 2.0f);
    renderable.Sprite.setPosition(setup.Position);
    return renderable;
}
Collidable& SetupCollidable(const Entity::Setup& setup, const sf::IntRect& collisionRect){
    return setup.Registry.emplace<Collidable>(
        setup.ThisEntity, 
        collisionRect, 
        setup.Source, 
        setup.CLayer.value_or(
            CollisionLayer::Enemy
        ),
        setup.CMask.value_or(
            GetCollisionMask({
                CollisionLayer::Player, CollisionLayer::PlayerProjectile
            })
        ),
        OnCollision);
}
Health& SetupHealth(const Entity::Setup& setup, float maxHealth, Callback callback){
    return setup.Registry.emplace<Health>(setup.ThisEntity, maxHealth, callback);
}
Behavior& SetupBehavior(const Entity::Setup& setup, Callback callback){
    return setup.Registry.emplace<Behavior>(setup.ThisEntity, callback);
}
void SetupMovement(const Entity::Setup& setup, float moveSpeed){
    setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * moveSpeed);
    setup.Registry.emplace<MaxSpeed>(setup.ThisEntity, moveSpeed);
}
void SetupOffscreenLifetime(const Entity::Setup& setup, float expireTimeInSeconds){
    setup.Registry.emplace<ScreenTrigger>(setup.ThisEntity, Entity::RemoveOffscreenLifetime, Entity::AddOffscreenLifetime, expireTimeInSeconds);
}
void OnCollision(Collision& collision) {
    if (collision.Registry.valid(collision.ThisEntity) && collision.Registry.valid(collision.OtherEntity)) {
        spdlog::trace("Enemy::OnCollision invoked between entity {} and entity {}", 
        static_cast<int>(collision.ThisEntity), static_cast<int>(collision.OtherEntity));
    }
}
}
}
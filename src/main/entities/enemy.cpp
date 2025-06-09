#include "pch.hpp"
#include "enemy.hpp"

#include "system/onegungame.hpp"
#include "entities/entity.hpp"
#include "entities/projectile_types.hpp"

void Enemy::Update(entt::registry &registry) {
    registry.view<Behavior>().each(
        [&](auto entity, Behavior &behavior) {
            behavior.Act(registry, entity);
        }
    );
}
entt::entity Enemy::Create(Setup& setup) {
    setup.ThisEntity = setup.Registry.create();
    
    return setup.ThisEntity;
}
Renderable& Enemy::SetupRenderable(const Setup& setup, OneGunGame::Images imageID, const sf::IntRect& textureRect){
    auto &renderable = setup.Registry.emplace<Renderable>(setup.ThisEntity, 
        OneGunGame::GetTexture(imageID), 50);
    renderable.Sprite.setTextureRect(textureRect);
    renderable.Sprite.setOrigin(static_cast<sf::Vector2f>(renderable.Sprite.getTextureRect().size) / 2.0f);
    renderable.Sprite.setPosition(setup.Position);
    return renderable;
}
Collidable& Enemy::SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect){
    sf::IntRect centeredRect = collisionRect;
    centeredRect.position -= centeredRect.size / 2;
    return setup.Registry.emplace<Collidable>(setup.ThisEntity, collisionRect, setup.Source, OneGunGame::CollisionLayer::Enemy,
        static_cast<OneGunGame::CollisionLayer>(OneGunGame::Player | OneGunGame::Projectile),
        OnCollision);
}
Health& Enemy::SetupHealth(const Setup& setup, float maxHealth){
    return setup.Registry.emplace<Health>(setup.ThisEntity, maxHealth);
}
void Enemy::SetupMovement(const Setup& setup, float moveSpeed){
    setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * moveSpeed);
    setup.Registry.emplace<MaxSpeed>(setup.ThisEntity, moveSpeed);
}
void Enemy::SetupOffscreenLifetime(const Setup& setup, float expireTimeInSeconds){
    setup.Registry.emplace<ScreenTrigger>(setup.ThisEntity, Entity::RemoveOffscreenLifetime, Entity::AddOffscreenLifetime, expireTimeInSeconds);
}
entt::entity Enemy::Fire(entt::registry &registry, entt::entity sourceEntity) {
    auto fireComponent = registry.try_get<Fireable>(sourceEntity);
    if (!fireComponent) {
        spdlog::warn("Source entity {} does not have a Fireable component", static_cast<int>(sourceEntity));
        return entt::null;
    }
    if (!fireComponent->Fire())
        return entt::null;
    auto weaponComponent = registry.try_get<Projectile::Weapon>(sourceEntity);
    if (!weaponComponent) {
        spdlog::warn("Source entity {} does not have a Weapon component", static_cast<int>(sourceEntity));
        return entt::null;
    }
    spdlog::trace("Player firing projectile");
    auto projectileType = weaponComponent->GetBulletType();
    spdlog::trace("Projectile type: {}", static_cast<int>(projectileType));
    Projectile::Setup setup{registry, registry.get<Renderable>(sourceEntity).Sprite.getPosition(),
    sf::Vector2f{0.0f, 1.0f}, sourceEntity};
    auto projectile = Projectile::Create(setup, projectileType);
    return projectile;
}
void Enemy::OnCollision(entt::registry &registry, entt::entity thisEntity, entt::entity otherEntity) {
    if (registry.valid(thisEntity) && registry.valid(otherEntity)) {
        spdlog::trace("Enemy::OnCollision invoked between entity {} and entity {}", 
        static_cast<int>(thisEntity), static_cast<int>(otherEntity));
    }
}

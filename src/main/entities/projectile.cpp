#include "pch.hpp"
#include "projectile.hpp"

#include "systems/onegungame.hpp"
#include "systems/components.hpp"
#include "entities/entity.hpp"
#include "entities/player.hpp"
#include "entities/enemy_types.hpp"

namespace Projectile {
    void Update(entt::registry &registry) {
        registry.view<Renderable, Velocity, Acceleration, Homing>().each(
            [&](Renderable& renderable, [[maybe_unused]]Velocity& velocity, Acceleration& accel, Homing& homing) {
                if (!registry.valid(homing.Target) || registry.any_of<Destructing, Dying>(homing.Target)) {
                    // Try to find new target
                    std::pair<entt::entity, float> closestTarget = {entt::null, std::numeric_limits<float>::max()};
                    registry.view<Renderable, Enemy::Component>().each(
                        [&](entt::entity targetEntity, Renderable& targetRenderable, [[maybe_unused]]Enemy::Component& enemyComponent) {
                            float distSquared = OneGunGame::GetDistanceSquared(renderable.Sprite.getPosition(), targetRenderable.Sprite.getPosition());
                            if (distSquared < closestTarget.second) {
                                closestTarget = {targetEntity, distSquared};
                            }
                        }
                    );
                    homing.Target = closestTarget.first;
                    return;
                }
                auto& otherRenderable = registry.get<Renderable>(homing.Target);

                sf::Vector2f desiredVector = (otherRenderable.Sprite.getPosition() - renderable.Sprite.getPosition()).normalized() * velocity.Value.length();
                accel.Value += (desiredVector - velocity.Value).normalized() * homing.RotationForce;

                renderable.Sprite.setRotation(-velocity.Value.angleTo({0.0f, -1.0f}));
            }
        );
    }

    entt::entity Create(Setup& setup)
    {
        entt::entity entity = setup.Registry.create();

        return entity;
    }

    Renderable& SetupRenderable(const Setup& setup, OneGunGame::Images imageID, const sf::IntRect& textureRect) {
        auto &renderable = setup.Registry.emplace<Renderable>(
            setup.ThisEntity, 
            OneGunGame::GetTexture(imageID), 
            25
        );
        renderable.Sprite.setTextureRect(textureRect);
        renderable.Sprite.setOrigin(
            {textureRect.size.x / 2.0f, textureRect.size.y / 2.0f}
        );
        renderable.Sprite.setPosition(setup.Position);
        return renderable;
    }
    Collidable& SetupCollidable(const Setup& setup, const sf::IntRect& collisionRect){
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

    void SetupMovement(const Setup& setup, float moveSpeed){
        setup.Registry.emplace<Velocity>(setup.ThisEntity);
        setup.Registry.emplace<Acceleration>(
            setup.ThisEntity, 
            setup.Direction * moveSpeed
        );
        setup.Registry.emplace<MaxSpeed>(setup.ThisEntity, moveSpeed);
    }

    float GetProjectileDamage(entt::registry &registry, entt::entity projectileEntity, float baseDamage) {
        float basePower = Entity::GetBasePower(registry, registry.get<Collidable>(projectileEntity).Source);
        return baseDamage * basePower;
    }
}
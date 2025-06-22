#include "pch.hpp"
#include "projectile.hpp"

#include "system/onegungame.hpp"
#include "system/components.hpp"
#include "entities/entity.hpp"
#include "entities/player.hpp"
#include "entities/enemy_types.hpp"

namespace Projectile {
    void Update(entt::registry &registry) {
        registry.view<Renderable, Velocity, Homing>().each(
            [&](Renderable& renderable, Velocity& velocity, Homing& homing) {
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
                    homing.Reset();
                    return;
                }
                auto& otherRenderable = registry.get<Renderable>(homing.Target);

                // Track the target
                sf::Angle angleToTarget = renderable.Sprite.getPosition().angleTo(otherRenderable.Sprite.getPosition());
                sf::Angle currentHeading = renderable.Sprite.getRotation();
                sf::Angle rotationAngle = homing.GetRotationAngle(angleToTarget - currentHeading);
                renderable.Sprite.rotate(rotationAngle);
                velocity.Value = velocity.Value.rotatedBy(rotationAngle);
                homing.Reset();
            }
        );
    }

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
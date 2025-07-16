#include "pch.hpp"
#include "entities/enemy_types.hpp"
#include "entities/entity.hpp"
#include "entities/projectile_types.hpp"

namespace Enemy::Comet {
    static constexpr const char* Name = "Comet";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionBlue;
    static constexpr sf::IntRect TextureRect = {{512, 96}, {32, 32}};
    static constexpr sf::IntRect TrailTextureRect = {{64, 352}, {32, 48}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {32, 32}};
    static constexpr sf::Vector2f DefaultScale{2.0f, 2.0f};
    static constexpr float MaxHealth = 100.0f;
    static constexpr float MoveSpeed = 4.0f;
    static constexpr float OffscreenLifetime = 2.5f;
    static constexpr size_t TotalFrames = 4U;
    static constexpr size_t TrailTotalFrames = 4U;
    static constexpr float FrameTimeInSeconds = 0.05f;
    static constexpr float TrailFrameTimeInSeconds = 0.03f;
    static constexpr size_t DeathProjectiles = 8U;

    void Create(const Setup& setup){
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        auto& renderable = SetupRenderable(setup, ImageID, TextureRect);
        renderable.Sprite.setScale(DefaultScale);
        renderable.Sprite.rotate(setup.Direction.angle());
        SetupCollidable(setup, CollisionRect);
        SetupMovement(setup, MoveSpeed);
        auto& health = SetupHealth(setup, MaxHealth);
        health.OnDeath = Death;
        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime);

        setup.Registry.emplace<Enemy::Behavior>(setup.ThisEntity, Behavior);
        setup.Registry.emplace<Animation>(setup.ThisEntity, renderable.Sprite.getTextureRect().position, TotalFrames, sf::seconds(FrameTimeInSeconds));

        auto trailSetup = Enemy::Setup{
            setup.Registry, 
            sf::Vector2f{}, 
            setup.Direction, 
            std::nullopt,
            std::nullopt,
            setup.Source, 
            setup.Registry.create()
        };
        SetupMovement(trailSetup, MoveSpeed);
        trailSetup.Position = setup.Position - setup.Direction.componentWiseMul(static_cast<sf::Vector2f>(renderable.Sprite.getTextureRect().size).componentWiseMul(renderable.Sprite.getScale()));

        auto& trailRenderable = SetupRenderable(trailSetup, ImageID, TrailTextureRect);
        trailRenderable.Sprite.setScale(DefaultScale);
        trailRenderable.Sprite.rotate(setup.Direction.angle() + sf::radians(OneGunGame::HalfPi));
        setup.Registry.emplace<Animation>(trailSetup.ThisEntity, trailRenderable.Sprite.getTextureRect().position, TrailTotalFrames, sf::seconds(TrailFrameTimeInSeconds));
        Entity::SetupOffscreenLifetime(setup.Registry, trailSetup.ThisEntity, OffscreenLifetime);
        setup.Registry.emplace<Attachment>(trailSetup.ThisEntity, setup.ThisEntity, 
            [](entt::registry& registry, entt::entity thisEntity) {
                registry.emplace<Destructing>(thisEntity);
            }
        );
    }

    void Behavior(entt::registry &registry, entt::entity thisEntity){
        if (registry.valid(thisEntity)){
            spdlog::trace("Comet Behavior invoked for entity {}", static_cast<int>(thisEntity));
        }
    }

    void Death(entt::registry &registry, entt::entity thisEntity){
        registry.emplace<Destructing>(thisEntity);

        /* DEATH STUFF */
        auto &renderable = registry.get<Renderable>(thisEntity);
        auto &velocity = registry.get<Velocity>(thisEntity);
        auto &collidable = registry.get<Collidable>(thisEntity);

        /* LAUNCH ICE PROJECTILES */
        for (auto i : std::ranges::iota_view{0U, DeathProjectiles}) {
            sf::Vector2f direction = velocity.Value.rotatedBy(
                sf::radians(
                    (OneGunGame::Pi * 2) / 
                    (DeathProjectiles==0?1:DeathProjectiles) * i
                )
            ).normalized();

            Projectile::Setup setup{
                registry, 
                renderable.Sprite.getPosition(), 
                direction, 
                OneGunGame::NeutralProjectile,
                static_cast<uint8_t>(
                    OneGunGame::Player | OneGunGame::Enemy | 
                    OneGunGame::Obstacle
                ),
                collidable.Source
            };
            Projectile::Create(setup, Projectile::Type::Ice);
        }

        /* CREATE DEATH EXPLOSION */
        //Explosion::Setup setup{registry, renderable.Sprite.getPosition(), velocity.Value};
        //Explosion::Create(setup, Explosion::Type::AsteroidDeath);
    }
}
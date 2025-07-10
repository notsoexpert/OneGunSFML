#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "system/onegungame.hpp"
#include "entities/entity.hpp"
#include "system/components.hpp"
#include "entities/explosion_types.hpp"

namespace Projectile::Bomb {
    static constexpr const char* Name = "Bomb";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{576, 0}, {64, 64}};
    static constexpr sf::IntRect WaveTextureRect = {{0, 128},{32, 32}};
    static constexpr sf::Vector2f WaveStartScale = {0.1f, 0.1f};
    static constexpr sf::Vector2f WaveEndScale = {100.0f, 100.0f};
    static constexpr float WaveLifeTimeInSeconds = 1.0f;
    static constexpr float DamageFactor = 100.0f;
    static constexpr float MoveSpeed = 2.5f;
    static constexpr float LifeTimeInSeconds = 2.0f;
    static constexpr float BoomDelayInSeconds = 0.45f;
    static constexpr uint8_t Specification = Flags::Explode;

    void Create(const Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        SetupRenderable(setup, ImageID, TextureRect);
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed);
        setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Bomb, Specification, 0.0f);
        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(LifeTimeInSeconds), Death);
    }
    
    void CreateBoomWave(entt::registry &registry, entt::entity boomTicker) {
        registry.emplace<Destructing>(boomTicker);

        auto waveEntity = registry.create();
        auto& waveRenderable = registry.emplace<Renderable>(waveEntity, OneGunGame::GetTexture(OneGunGame::Images::ExplosionYellow));
        waveRenderable.Sprite.setTextureRect(WaveTextureRect);
        waveRenderable.Sprite.setPosition(registry.get<Renderable>(boomTicker).Sprite.getPosition());
        waveRenderable.Sprite.setScale(WaveStartScale);
        waveRenderable.Sprite.setOrigin({WaveTextureRect.size.x / 2.0f, WaveTextureRect.size.y / 2.0f});

        registry.emplace<Scaling>(waveEntity, WaveStartScale, WaveEndScale, sf::seconds(WaveLifeTimeInSeconds));
        registry.emplace<Fading>(waveEntity, static_cast<uint8_t>(255), static_cast<uint8_t>(0), sf::seconds(WaveLifeTimeInSeconds));
        registry.emplace<Rotating>(waveEntity, sf::radians(4*OneGunGame::Pi));

        sf::IntRect collisionRect = {{0, 0}, {16,16}};
        OneGunGame::CollisionLayer mask = OneGunGame::GetHitMask(OneGunGame::CollisionLayer::Projectile);
        registry.emplace<Collidable>(waveEntity, collisionRect, entt::null, OneGunGame::CollisionLayer::Projectile, mask,
            Explosion::OnCollision);
        registry.emplace<HitLimiting>(waveEntity);
        registry.emplace<Explosion::Component>(waveEntity, Explosion::Type::Bomb, DamageFactor);
        registry.emplace<Lifetime>(waveEntity, sf::seconds(WaveLifeTimeInSeconds), [](entt::registry& registry, entt::entity thisEntity) { registry.emplace<Destructing>(thisEntity); });
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        registry.emplace<Destructing>(thisEntity);

        auto pos = registry.get<Renderable>(thisEntity).Sprite.getPosition();

        Explosion::Setup explosionSetup{
            registry,
            pos,
            registry.get<Velocity>(thisEntity).Value,
            OneGunGame::GetPlayerEntity()
        };
        Explosion::Prebomb::Create(explosionSetup);

        auto boomTickerEntity = registry.create();
        auto& renderable = registry.emplace<Renderable>(boomTickerEntity, OneGunGame::GetTexture(OneGunGame::Images::Unknown), 1000);
        renderable.Sprite.setPosition(pos);
        registry.emplace<Lifetime>(boomTickerEntity, sf::seconds(BoomDelayInSeconds), CreateBoomWave);
    }

}
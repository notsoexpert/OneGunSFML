#include "pch.hpp"
#include "entities/explosion_types.hpp"

#include "system/components.hpp"
#include "system/onegungame.hpp"

namespace Explosion::Bomb {
    static constexpr const char* Name = "Bomb";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionYellow;
    static constexpr sf::IntRect TextureRect = {{512, 384}, {32, 32}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {32, 32}};
    static constexpr size_t TotalFrames = 4U;
    static constexpr float FrameTimeInSeconds = 0.05f;
    static constexpr float LifetimeDuration = TotalFrames * FrameTimeInSeconds;
    static constexpr sf::Vector2f TargetScale = {4.0f, 4.0f};
    static constexpr float ScaleTimeInSeconds = FrameTimeInSeconds * 2.0f;
    
    static constexpr float WaveDamageFactor = 100.0f;
    static constexpr sf::IntRect WaveTextureRect = {{0, 128},{32, 32}};
    static constexpr sf::Vector2f WaveStartScale = {0.1f, 0.1f};
    static constexpr sf::Vector2f WaveEndScale = {100.0f, 100.0f};
    static constexpr float WaveLifeTimeInSeconds = 1.0f;

    void Create(Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        setup.ThisEntity = Explosion::Create(setup);

        auto& renderable = SetupRenderable(setup, ImageID, TextureRect);
        //SetupCollidable(setup, CollisionRect); // Currently only boom wave does damage (see Projectile::Bomb)
        setup.Registry.emplace<Component>(setup.ThisEntity, Explosion::Type::Bomb);
        setup.Registry.emplace<Animation>(setup.ThisEntity, renderable.Sprite.getTextureRect().position, TotalFrames, sf::seconds(FrameTimeInSeconds));
        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(LifetimeDuration));
        setup.Registry.emplace<Scaling>(setup.ThisEntity, renderable.Sprite.getScale(), TargetScale, sf::seconds(ScaleTimeInSeconds));
        setup.Registry.emplace<HitLimiting>(setup.ThisEntity);
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
        registry.emplace<Collidable>(
            waveEntity, 
            collisionRect, 
            OneGunGame::GetPlayerEntity(), 
            OneGunGame::CollisionLayer::PlayerProjectile, 
            static_cast<uint8_t>(
                OneGunGame::Enemy | OneGunGame::EnemyProjectile | 
                OneGunGame::Obstacle | OneGunGame::NeutralProjectile
            ),
            Explosion::OnCollision
        );
        registry.emplace<HitLimiting>(waveEntity);
        registry.emplace<Explosion::Component>(waveEntity, Explosion::Type::Bomb, WaveDamageFactor);
        registry.emplace<Lifetime>(waveEntity, sf::seconds(WaveLifeTimeInSeconds), [](entt::registry& registry, entt::entity thisEntity) { registry.emplace<Destructing>(thisEntity); });
    }

}
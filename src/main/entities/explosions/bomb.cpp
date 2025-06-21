#include "pch.hpp"
#include "entities/explosion_types.hpp"

#include "system/components.hpp"

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

    void Create(Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        setup.ThisEntity = Explosion::Create(setup);

        auto& renderable = SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        setup.Registry.emplace<Component>(setup.ThisEntity, Explosion::Type::VisualOnly);
        setup.Registry.emplace<Animation>(setup.ThisEntity, renderable.Sprite.getTextureRect().position, TotalFrames, sf::seconds(FrameTimeInSeconds));
        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(LifetimeDuration));
        setup.Registry.emplace<Scaling>(setup.ThisEntity, renderable.Sprite.getScale(), TargetScale, sf::seconds(ScaleTimeInSeconds));
        setup.Registry.emplace<HitLimiting>(setup.ThisEntity);
    }

}
#include "pch.hpp"
#include "entities/explosion_types.hpp"

#include "system/components.hpp"

namespace Explosion::Missile {
    static constexpr const char* Name = "Missile Explosion";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionViolet;
    static constexpr sf::IntRect TextureRect = {{512, 448}, {32, 32}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {32, 32}};
    static constexpr size_t TotalFrames = 4U;
    static constexpr float FrameTimeInSeconds = 0.05f;
    static constexpr float LifetimeDuration = TotalFrames*FrameTimeInSeconds;

    void Create(Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        setup.ThisEntity = Explosion::Create(setup);

        auto& renderable = SetupRenderable(setup, ImageID, TextureRect);
        setup.Registry.emplace<Component>(setup.ThisEntity, Explosion::Type::VisualOnly);
        setup.Registry.emplace<Animation>(setup.ThisEntity, renderable.Sprite.getTextureRect().position, TotalFrames, sf::seconds(FrameTimeInSeconds));
        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(LifetimeDuration));
        setup.Registry.emplace<Collidable>(setup.ThisEntity, CollisionRect, setup.Position, setup.Velocity, setup.Source);
    }

}
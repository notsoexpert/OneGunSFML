#include "pch.hpp"
#include "entities/explosion_types.hpp"

#include "components/renderable.hpp"
#include "components/animation.hpp"
#include "components/collidable.hpp"
#include "components/lifetime.hpp"

namespace OneGunGame{
namespace Explosion::Missile {
    static constexpr const char* Name = "Missile Explosion";
    static constexpr Images ImageID = Images::ExplosionYellow;
    static constexpr sf::IntRect TextureRect = {{192, 160}, {32, 32}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {32, 32}};
    static constexpr size_t TotalFrames = 4U;
    static constexpr float FrameTimeInSeconds = 0.05f;
    static constexpr float LifetimeDuration = TotalFrames*FrameTimeInSeconds;
    static constexpr float BaseDamage = 5.0f;

    void Create(Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        setup.ThisEntity = Explosion::Create(setup);

        auto& renderable = SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        setup.Registry.emplace<Component>(setup.ThisEntity, Explosion::Type::Missile, BaseDamage);
        setup.Registry.emplace<Animation>(setup.ThisEntity, renderable.Sprite.getTextureRect().position, TotalFrames, sf::seconds(FrameTimeInSeconds));
        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(LifetimeDuration));
    }

}
}
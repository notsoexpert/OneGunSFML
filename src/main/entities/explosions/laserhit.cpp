#include "pch.hpp"
#include "entities/explosion_types.hpp"

#include "systems/onegungame.hpp"

#include "components/renderable.hpp"
#include "components/animation.hpp"
#include "components/lifetime.hpp"

namespace OneGunGame{
namespace Explosion::LaserHit {
    static constexpr const char* Name = "Laser Hit";
    static constexpr std::array<Images, 3> ImageID = {
            Images::ExplosionGreen, 
            Images::ExplosionYellow, 
            Images::ExplosionRed
    };
    static constexpr sf::IntRect TextureRect = {{192, 64}, {32, 32}};
    static constexpr size_t TotalFrames = 4U;
    static constexpr float FrameTimeInSeconds = 0.05f;
    static constexpr float LifetimeDuration = TotalFrames*FrameTimeInSeconds;

    void Create(Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        setup.ThisEntity = Explosion::Create(setup);

        auto& renderable = SetupRenderable(setup, ImageID[0], TextureRect);
        setup.Registry.emplace<Component>(setup.ThisEntity, Explosion::Type::VisualOnly);
        setup.Registry.emplace<Animation>(setup.ThisEntity, renderable.Sprite.getTextureRect().position, TotalFrames, sf::seconds(FrameTimeInSeconds));
        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(LifetimeDuration));
    }

    void SetImageIndex(Renderable& renderable, size_t tier) {
        if (tier > 2) {
            spdlog::error("Projectile::Laser::SetImageWithTier - Tier {} out of range", tier);
            return;
        }

        renderable.Sprite.setTexture(GetTexture(ImageID.at(tier)));
    }
}
}
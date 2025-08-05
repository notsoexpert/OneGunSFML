#include "pch.hpp"
#include "entities/explosion_types.hpp"

#include "systems/onegungame.hpp"
#include "components/renderable.hpp"
#include "components/animation.hpp"
#include "components/lifetime.hpp"

namespace OneGunGame{
namespace Explosion::Prebomb {
    static constexpr const char* Name = "Prebomb";
    static constexpr Images ImageID = Images::ExplosionYellow;
    static constexpr sf::IntRect TextureRect = {{192, 224}, {32, 32}};
    static constexpr size_t TotalFrames = 4U;
    static constexpr int AnimationDirection = -1;
    static constexpr float FrameTimeInSeconds = 0.05f;
    static constexpr float LifetimeDuration = TotalFrames * FrameTimeInSeconds;

    void Create(Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        setup.ThisEntity = Explosion::Create(setup);

        auto& renderable = SetupRenderable(setup, ImageID, TextureRect);
        auto texRect = renderable.Sprite.getTextureRect();
        setup.Registry.emplace<Component>(setup.ThisEntity, Explosion::Type::VisualOnly);
        auto& anim = setup.Registry.emplace<Animation>(setup.ThisEntity, renderable.Sprite.getTextureRect().position, TotalFrames, sf::seconds(FrameTimeInSeconds), AnimationDirection);
        anim.SetFrame(TotalFrames - 1U, texRect);
        renderable.Sprite.setTextureRect(texRect);
        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(LifetimeDuration), Death);
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        registry.emplace<Destructing>(thisEntity);

        Explosion::Setup explosionSetup{
            registry,
            registry.get<Renderable>(thisEntity).Sprite.getPosition(),
            {0.0f, 0.0f}
        };
        Explosion::Bomb::Create(explosionSetup);
    }

}
}
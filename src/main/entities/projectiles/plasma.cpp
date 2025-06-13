#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"
#include "system/components.hpp"

namespace Projectile::Plasma {
    static constexpr const char* Name = "Plasma";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::ExplosionYellow;
    static constexpr sf::IntRect TextureRect = {{512, 256}, {32, 32}};
    static constexpr sf::IntRect CollisionRect = {{0, 0}, {32, 32}};
    static constexpr size_t Specification = Flags::Burn;
    static constexpr size_t TotalFrames = 4U;
    static constexpr float BaseDamage = 1.0f;
    static constexpr float MoveSpeed = 3.0f;
    static constexpr float LifetimeDuration = 1.25f;
    static constexpr float FrameTimeInSeconds = 0.05f;
    static constexpr float ScaleTimeInSeconds = 1.25f;
    static constexpr float FadeTimeInSeconds = 1.25f;
    static constexpr float TargetScale = 3.0f;
    static constexpr uint8_t TargetAlpha = 0U;

    void Create(const Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        auto& renderable = SetupRenderable(setup, ImageID, TextureRect);
        SetupCollidable(setup, CollisionRect);
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed);

        setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Plasma, 
            Specification, GetProjectileDamage(setup.Registry, setup.ThisEntity, BaseDamage));
        setup.Registry.emplace<Animation>(setup.ThisEntity, renderable.Sprite.getTextureRect().position, TotalFrames, sf::seconds(FrameTimeInSeconds));
        setup.Registry.emplace<Rotating>(setup.ThisEntity, sf::radians(2*OneGunGame::Pi));
        setup.Registry.emplace<Scaling>(setup.ThisEntity, renderable.Sprite.getScale(), sf::Vector2f{TargetScale, TargetScale}, sf::seconds(ScaleTimeInSeconds));
        setup.Registry.emplace<Fading>(setup.ThisEntity, renderable.Sprite.getColor().a, TargetAlpha, sf::seconds(FadeTimeInSeconds));
        setup.Registry.emplace<Burning>(setup.ThisEntity);
        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(LifetimeDuration));
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        auto &component = registry.get<Component>(thisEntity);

        if (component.CompareFlags(Flags::Explode)) {
            spdlog::info("Entity {} exploding!", static_cast<int>(thisEntity));
        }
    }
}
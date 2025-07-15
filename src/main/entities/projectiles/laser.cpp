#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"
#include "system/components.hpp"

#include "entities/explosion_types.hpp"

namespace Projectile::Laser {
    static constexpr uint8_t Tiers = 3;
    static constexpr std::array<const char*, Tiers> Name = 
    {"Green Laser", "Yellow Laser", "Red Laser"};
    static constexpr std::array<OneGunGame::Images, Tiers> ImageID = 
    {OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet};
    static constexpr std::array<sf::IntRect, Tiers> TextureRect = 
    {sf::IntRect{{256, 0}, {64, 64}}, sf::IntRect{{320, 0}, {64, 64}}, sf::IntRect{{384, 0}, {64, 64}}};
    static constexpr std::array<sf::IntRect, Tiers> CollisionRect = 
    {sf::IntRect{{0, 0}, {8, 17}}, sf::IntRect{{0, 0}, {10, 26}}, sf::IntRect{{0, 0}, {12, 36}}};
    static constexpr std::array<float, Tiers> DamageFactor = 
    {2.0f, 2.0f, 2.0f};
    static constexpr std::array<float, Tiers> MoveSpeed = 
    {6.0f, 6.0f, 6.0f};
    static constexpr std::array<float, Tiers> OffscreenLifetime = 
    {1.0f, 1.0f, 1.0f};
    static constexpr std::array<uint8_t, Tiers> Specification = 
    {Flags::Destruct | Flags::Split, Flags::Destruct | Flags::Split, Flags::Destruct | Flags::Split};

    static constexpr size_t SplitProjectiles = 2;
    static constexpr size_t TotalSplits = 2;
    static constexpr float SplitDamageFactor = 0.50f;

    void Create(const Setup& setup) {
        auto index = setup.Tier;
        spdlog::trace("Setting up {} at ({}, {})", Name.at(index), setup.Position.x, setup.Position.y);
        
        SetupRenderable(setup, ImageID.at(index), TextureRect.at(index));
        SetupCollidable(setup, CollisionRect.at(index));
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed.at(index));

        auto &component = setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Laser, 
            Specification.at(index), DamageFactor.at(index));
        component.Tier = index;
        setup.Registry.emplace<HitLimiting>(setup.ThisEntity);
        setup.Registry.emplace<Splitting>(setup.ThisEntity, TotalSplits, setup.Direction);

        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime.at(index));
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        registry.emplace_or_replace<Destructing>(thisEntity);

        auto &component = registry.get<Component>(thisEntity);

        /* GET RELEVANT COMPONENTS */
        auto &renderable = registry.get<Renderable>(thisEntity);
        auto &velocity = registry.get<Velocity>(thisEntity);

        /* CREATE EXPLOSION */
        Explosion::Setup explosionSetup{
            registry,
            renderable.Sprite.getPosition(),
            velocity.Value,
            registry.get<Collidable>(thisEntity).Source
        };
        Explosion::LaserHit::Create(explosionSetup);
        Explosion::LaserHit::SetImageIndex(registry.get<Renderable>(explosionSetup.ThisEntity), component.Tier);


        /* SPLIT INTO WEAKER LASERS */
        auto& splitting = registry.get<Splitting>(thisEntity);
        if (!splitting.CanSplit()) {
            return;
        }

        for (auto i : std::ranges::iota_view{0U, SplitProjectiles}) {
            sf::Angle rotation = sf::radians(OneGunGame::HalfPi / 2.0f * (i / 2 + 1) * (i % 2 == 0 ? 1.0f : -1.0f));
            sf::Vector2f newDirection = splitting.OriginalDirection.rotatedBy(rotation).normalized();

            Projectile::Setup setup{registry, renderable.Sprite.getPosition(), newDirection, registry.get<Collidable>(thisEntity).Source};
            Projectile::Create(setup, Projectile::Type::Laser);

            Renderable& splitRenderable = registry.get<Renderable>(setup.ThisEntity);
            splitRenderable.Sprite.rotate(rotation);
            HitLimiting& splitHitLimiting = registry.get<HitLimiting>(setup.ThisEntity);
            splitHitLimiting.HitEntities = registry.get<HitLimiting>(thisEntity).HitEntities;
            Component& splitComponent = registry.get<Component>(setup.ThisEntity);
            splitComponent.DamageFactor = component.DamageFactor * SplitDamageFactor;
            splitComponent.Tier = static_cast<uint8_t>(std::clamp<int>(component.Tier - 1, 0, Tiers - 1));
            splitRenderable.Sprite.setTextureRect(TextureRect.at(splitComponent.Tier));
            Splitting& splitSplitting = registry.get<Splitting>(setup.ThisEntity);
            splitSplitting.CurrentSplits = splitting.CurrentSplits + 1;
            splitSplitting.OriginalDirection = splitting.OriginalDirection;
        }

    }
}
#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"
#include "system/components.hpp"

#include "entities/explosion_types.hpp"

namespace Projectile::Laser {
    static constexpr std::array<const char*, 3> Name = 
    {"Green Laser", "Yellow Laser", "Red Laser"};
    static constexpr std::array<OneGunGame::Images, 3> ImageID = 
    {OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet};
    static constexpr std::array<sf::IntRect, 3> TextureRect = 
    {sf::IntRect{{256, 0}, {64, 64}}, sf::IntRect{{320, 0}, {64, 64}}, sf::IntRect{{384, 0}, {64, 64}}};
    static constexpr std::array<sf::IntRect, 3> CollisionRect = 
    {sf::IntRect{{0, 0}, {8, 17}}, sf::IntRect{{0, 0}, {10, 26}}, sf::IntRect{{0, 0}, {12, 36}}};
    static constexpr std::array<float, 3> BaseDamage = 
    {2.0f, 2.0f, 2.0f};
    static constexpr std::array<float, 3> MoveSpeed = 
    {6.0f, 6.0f, 6.0f};
    static constexpr std::array<float, 3> OffscreenLifetime = 
    {1.0f, 1.0f, 1.0f};
    static constexpr std::array<size_t, 3> Specification = 
    {Flags::Destruct | Flags::Split, Flags::Destruct | Flags::Split, Flags::Destruct | Flags::Split};

    static constexpr size_t SplitProjectiles = 2;
    static constexpr size_t TotalSplits = 2;
    static constexpr float SplitDamageFactor = 0.50f;

    size_t GetTier(float basePower){
        if (basePower < 5.0f){
            return 0;
        }
        if (basePower < 10.0f){
            return 1;
        }
        return 2;
    }

    void Create(const Setup& setup) {
        float power = Entity::GetBasePower(setup.Registry, setup.Source);
        size_t index = GetTier(power);
        spdlog::trace("Setting up {} at ({}, {})", Name.at(index), setup.Position.x, setup.Position.y);
        
        SetupRenderable(setup, ImageID.at(index), TextureRect.at(index));
        SetupCollidable(setup, CollisionRect.at(index));
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed.at(index));

        setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Laser, 
            Specification.at(index), BaseDamage.at(index));
        setup.Registry.emplace<HitLimiting>(setup.ThisEntity);
        setup.Registry.emplace<Splitting>(setup.ThisEntity, TotalSplits);

        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime.at(index));
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        auto &component = registry.get<Component>(thisEntity);

        if (component.CompareFlags(Flags::Explode)) {
            spdlog::info("Entity {} exploding!", static_cast<int>(thisEntity));
        }

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
        Explosion::VisualOnly::LaserHit::Create(explosionSetup);
        Explosion::VisualOnly::LaserHit::SetImageWithTier(registry.get<Renderable>(explosionSetup.ThisEntity), GetTier(component.BaseDamage));

        registry.emplace_or_replace<Destructing>(thisEntity);

        /* SPLIT INTO WEAKER LASERS */
        auto& splitting = registry.get<Splitting>(thisEntity);
        if (!splitting.CanSplit()) {
            return;
        }

        for (int i = 0; i < SplitProjectiles; i++) {
            sf::Angle rotation = sf::radians(OneGunGame::HalfPi / 2.0f * (i / 2 + 1) * (i % 2 == 0 ? 1.0f : -1.0f) * (1 + splitting.CurrentSplits));
            sf::Vector2f newDirection = velocity.Value.rotatedBy(rotation).normalized();

            Projectile::Setup setup{registry, renderable.Sprite.getPosition(), newDirection, registry.get<Collidable>(thisEntity).Source};
            Projectile::Create(setup, Projectile::Type::Laser);

            Renderable& splitRenderable = registry.get<Renderable>(setup.ThisEntity);
            splitRenderable.Sprite.rotate(rotation);
            HitLimiting& splitHitLimiting = registry.get<HitLimiting>(setup.ThisEntity);
            splitHitLimiting.HitEntities = registry.get<HitLimiting>(thisEntity).HitEntities;
            Component& splitComponent = registry.get<Component>(setup.ThisEntity);
            splitComponent.BaseDamage = component.BaseDamage * SplitDamageFactor;
            splitRenderable.Sprite.setTextureRect(TextureRect.at(GetTier(splitComponent.BaseDamage)));
            Splitting& splitSplitting = registry.get<Splitting>(setup.ThisEntity);
            splitSplitting.CurrentSplits = splitting.CurrentSplits + 1;
        }

    }
}
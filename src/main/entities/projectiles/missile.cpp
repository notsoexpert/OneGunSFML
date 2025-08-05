#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"

#include "entities/explosion_types.hpp"

#include "components/renderable.hpp"
#include "components/transformation.hpp"
#include "components/animation.hpp"
#include "components/collidable.hpp"
#include "components/lifetime.hpp"

namespace OneGunGame{
namespace Projectile::Missile {
    static constexpr uint8_t Tiers = 2;
    static constexpr std::array<const char*, Tiers> Name = 
    {"Missile", "Homing Missile"};
    static constexpr std::array<Images, Tiers> ImageID = 
    {Images::SpriteSheet, Images::SpriteSheet};
    static constexpr std::array<sf::IntRect, Tiers> TextureRect = 
    {sf::IntRect{{448, 0}, {64, 64}}, sf::IntRect{{512, 0}, {64, 64}}};
    static constexpr std::array<sf::IntRect, Tiers> CollisionRect = 
    {sf::IntRect{{0, 0}, {8, 17}}, sf::IntRect{{0, 0}, {8, 17}}};
    static constexpr std::array<float, Tiers> DamageFactor = 
    {0.0f, 0.0f};
    static constexpr std::array<float, Tiers> MoveSpeed = 
    {6.0f, 5.0f};
    static constexpr std::array<float, Tiers> ProjectileLifetime = 
    {4.0f, 3.0f};
    static constexpr std::array<uint8_t, Tiers> Specification = 
    {Flags::Destruct | Flags::Explode, Flags::Destruct | Flags::Explode | Flags::Home};

    void Create(const Projectile::Setup& setup) {
        auto index = setup.Tier;
        spdlog::trace("Setting up {} at ({}, {})", Name.at(index), setup.Position.x, setup.Position.y);
        
        SetupRenderable(setup, ImageID.at(index), TextureRect.at(index));
        SetupCollidable(setup, CollisionRect.at(index));
        SetupMovement(setup, MoveSpeed.at(index));

        auto &component = setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Missile, 
            Specification.at(index), DamageFactor.at(index));
        component.Tier = index;

        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(ProjectileLifetime.at(index)), Death);

        if (component.CompareFlags(Flags::Home)) {
            auto& homing = setup.Registry.emplace<Homing>(setup.ThisEntity);
            homing.RotationForce = MoveSpeed.at(index) / 50.0f;
        }
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        registry.emplace<Destructing>(thisEntity);
        auto& collidable = registry.get<Collidable>(thisEntity);

        Explosion::Setup explosionSetup{
            registry,
            registry.get<Renderable>(thisEntity).Sprite.getPosition(),
            registry.get<Velocity>(thisEntity).Value,
            collidable.CollisionLayer,
            collidable.CollisionMask,
            collidable.Source
        };
        Explosion::Missile::Create(explosionSetup);
    }
}
}
#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"
#include "system/components.hpp"

#include "entities/explosion_types.hpp"

namespace Projectile::Missile {
    static constexpr std::array<const char*, 2> Name = 
    {"Missile", "Homing Missile"};
    static constexpr std::array<OneGunGame::Images, 2> ImageID = 
    {OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet};
    static constexpr std::array<sf::IntRect, 2> TextureRect = 
    {sf::IntRect{{448, 0}, {64, 64}}, sf::IntRect{{512, 0}, {64, 64}}};
    static constexpr std::array<sf::IntRect, 2> CollisionRect = 
    {sf::IntRect{{0, 0}, {8, 17}}, sf::IntRect{{0, 0}, {8, 17}}};
    static constexpr std::array<float, 2> BaseDamage = 
    {0.0f, 0.0f};
    static constexpr std::array<float, 2> MoveSpeed = 
    {6.0f, 5.0f};
    static constexpr std::array<float, 2> ProjectileLifetime = 
    {4.0f, 3.0f};
    static constexpr std::array<size_t, 2> Specification = 
    {Flags::Destruct | Flags::Explode, Flags::Destruct | Flags::Explode | Flags::Home};

    void Create(const Projectile::Setup& setup) {
        size_t index = 0U;

        auto weapComp = setup.Registry.try_get<Projectile::Weapon>(setup.Source);
        if (weapComp) {
            index = weapComp->TierOverride.value_or(0U);
        } else {
            spdlog::warn("MissileSetup - GetIndex - Source {} has no weapon component", static_cast<int>(setup.Source));
        }

        spdlog::trace("Setting up {} at ({}, {})", Name.at(index), setup.Position.x, setup.Position.y);
        
        SetupRenderable(setup, ImageID.at(index), TextureRect.at(index));
        SetupCollidable(setup, CollisionRect.at(index));
        SetupMovement(setup, MoveSpeed.at(index));

        setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Missile, 
            Specification.at(index), BaseDamage.at(index));

        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(ProjectileLifetime.at(index)), Death);

        auto& homing = setup.Registry.emplace<Homing>(setup.ThisEntity);
        homing.RotationForce = MoveSpeed.at(index) / 50.0f;
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        registry.emplace<Destructing>(thisEntity);

        Explosion::Setup explosionSetup{
            registry,
            registry.get<Renderable>(thisEntity).Sprite.getPosition(),
            registry.get<Velocity>(thisEntity).Value,
            registry.get<Collidable>(thisEntity).Source
        };
        Explosion::Missile::Create(explosionSetup);
    }
}
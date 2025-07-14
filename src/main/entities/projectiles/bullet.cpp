#include "pch.hpp"
#include "entities/projectile_types.hpp"
#include "entities/entity.hpp"

#include "system/components.hpp"

#include "entities/explosion_types.hpp"

namespace Projectile::Bullet {
    static constexpr uint8_t Tiers = 3;
    static constexpr std::array<const char*, Tiers> Name = 
    {"Bullet", "Large Bullet", "Huge Bullet"};
    static constexpr std::array<OneGunGame::Images, Tiers> ImageID = 
    {OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet};
    static constexpr std::array<sf::IntRect, Tiers> TextureRect = 
    {sf::IntRect{{64, 0}, {64, 64}}, sf::IntRect{{128, 0}, {64, 64}}, sf::IntRect{{192, 0}, {64, 64}}};
    static constexpr std::array<sf::IntRect, Tiers> CollisionRect = 
    {sf::IntRect{{0, 0}, {8, 17}}, sf::IntRect{{0, 0}, {10, 26}}, sf::IntRect{{0, 0}, {12, 36}}};
    static constexpr std::array<float, Tiers> DamageFactor = 
    {1.0f, 1.0f, 1.0f};
    static constexpr std::array<float, Tiers> MoveSpeed = 
    {10.0f, 10.0f, 10.0f};
    static constexpr std::array<float, Tiers> OffscreenLifetime = 
    {1.0f, 1.0f, 1.0f};
    static constexpr std::array<uint8_t, Tiers> Specification = 
    {Flags::Destruct, Flags::Destruct, Flags::Destruct};

    void Create(const Setup& setup) {
        auto index = setup.Tier;
        spdlog::trace("Setting up {} at ({}, {})", Name.at(index), setup.Position.x, setup.Position.y);

        SetupRenderable(setup, ImageID.at(index), TextureRect.at(index));
        SetupCollidable(setup, CollisionRect.at(index));
        SetupMovement(setup, MoveSpeed.at(index));

        auto &component = setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Bullet, 
            Specification.at(index), DamageFactor.at(index));
        component.Tier = index;

        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime.at(index));
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        registry.emplace_or_replace<Destructing>(thisEntity);

        Explosion::Setup explosionSetup{
            registry,
            registry.get<Renderable>(thisEntity).Sprite.getPosition(),
            registry.get<Velocity>(thisEntity).Value,
            registry.get<Collidable>(thisEntity).Source
        };
        Explosion::BulletHit::Create(explosionSetup);
    }
}

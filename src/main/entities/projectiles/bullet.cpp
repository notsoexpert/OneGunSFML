#include "pch.hpp"
#include "entities/projectile_types.hpp"
#include "entities/entity.hpp"

#include "system/components.hpp"

#include "entities/explosion_types.hpp"

namespace Projectile::Bullet {
    static constexpr std::array<const char*, 3> Name = 
    {"Bullet", "Large Bullet", "Huge Bullet"};
    static constexpr std::array<OneGunGame::Images, 3> ImageID = 
    {OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet};
    static constexpr std::array<sf::IntRect, 3> TextureRect = 
    {sf::IntRect{{64, 0}, {64, 64}}, sf::IntRect{{128, 0}, {64, 64}}, sf::IntRect{{192, 0}, {64, 64}}};
    static constexpr std::array<sf::IntRect, 3> CollisionRect = 
    {sf::IntRect{{0, 0}, {8, 17}}, sf::IntRect{{0, 0}, {10, 26}}, sf::IntRect{{0, 0}, {12, 36}}};
    static constexpr std::array<float, 3> BaseDamage = 
    {1.0f, 1.0f, 1.0f};
    static constexpr std::array<float, 3> MoveSpeed = 
    {10.0f, 10.0f, 10.0f};
    static constexpr std::array<float, 3> OffscreenLifetime = 
    {1.0f, 1.0f, 1.0f};
    static constexpr std::array<uint8_t, 3> Specification = 
    {Flags::Destruct, Flags::Destruct, Flags::Destruct};

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

        setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Bullet, 
            Specification.at(index), BaseDamage.at(index));

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

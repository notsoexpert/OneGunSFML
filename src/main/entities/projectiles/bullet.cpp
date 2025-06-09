#include "pch.hpp"
#include "entities/projectile_types.hpp"
#include "entities/entity.hpp"

#include "system/components.hpp"

namespace Projectile {
    static constexpr std::array<const char*, 3> Name = 
    {"Bullet", "Large Bullet", "Huge Bullet"};
    static constexpr std::array<OneGunGame::Images, 3> ImageID = 
    {OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet, OneGunGame::Images::SpriteSheet};
    static constexpr std::array<sf::IntRect, 3> TextureRect = 
    {sf::IntRect{{64, 0}, {64, 64}}, sf::IntRect{{128, 0}, {64, 64}}, sf::IntRect{{192, 0}, {64, 64}}};
    static constexpr std::array<sf::IntRect, 3> CollisionRect = 
    {sf::IntRect{{-4, -8}, {8, 17}}, sf::IntRect{{-5, -13}, {10, 26}}, sf::IntRect{{-6, -18}, {12, 36}}};
    static constexpr std::array<float, 3> BaseDamage = 
    {1.0f, 1.0f, 1.0f};
    static constexpr std::array<float, 3> MoveSpeed = 
    {10.0f, 10.0f, 10.0f};
    static constexpr std::array<float, 3> OffscreenLifetime = 
    {1.0f, 1.0f, 1.0f};
    static constexpr std::array<uint8_t, 3> Specification = 
    {Flags::Destruct, Flags::Destruct, Flags::Destruct};

    static size_t GetBulletTier(float basePower){
        if (basePower < 5.0f){
            return 0;
        }
        if (basePower < 10.0f){
            return 1;
        }
        return 2;
    }

    void BulletSetup(const Setup& setup) {
        float power = Entity::GetBasePower(setup.Registry, setup.Source);
        size_t index = GetBulletTier(power);
        spdlog::trace("Setting up {} at ({}, {})", Name.at(index), setup.Position.x, setup.Position.y);

        SetupRenderable(setup, ImageID.at(index), TextureRect.at(index));
        SetupCollidable(setup, CollisionRect.at(index));
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed.at(index));

        setup.Registry.emplace<Component>(setup.ThisEntity, Bullet, 
            Specification.at(index), GetProjectileDamage(setup.Registry, setup.ThisEntity, BaseDamage.at(index)));

        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime.at(index));
    }

    void BulletDestruction(entt::registry &registry, entt::entity thisEntity) {
        auto &component = registry.get<Component>(thisEntity);

        if (component.CompareFlags(Flags::Explode)) {
            spdlog::info("Entity {} exploding!", static_cast<int>(thisEntity));
        }
    }

}
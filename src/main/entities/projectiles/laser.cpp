#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "entities/entity.hpp"
#include "system/components.hpp"

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

    static size_t GetLaserTier(float basePower){
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
        size_t index = GetLaserTier(power);
        spdlog::trace("Setting up {} at ({}, {})", Name.at(index), setup.Position.x, setup.Position.y);
        
        SetupRenderable(setup, ImageID.at(index), TextureRect.at(index));
        SetupCollidable(setup, CollisionRect.at(index));
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed.at(index));

        setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Laser, 
            Specification.at(index), GetProjectileDamage(setup.Registry, setup.ThisEntity, BaseDamage.at(index)));

        Entity::SetupOffscreenLifetime(setup.Registry, setup.ThisEntity, OffscreenLifetime.at(index));
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        auto &component = registry.get<Component>(thisEntity);

        if (component.CompareFlags(Flags::Explode)) {
            spdlog::info("Entity {} exploding!", static_cast<int>(thisEntity));
        }
    }
}
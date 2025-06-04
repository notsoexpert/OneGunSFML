#pragma once

#include "system/constants.hpp"

namespace Projectile {
    enum Type {
        Bullet1 = 0,
        Bullet2,
        Bullet3,
        Laser1,
        Laser2,
        Laser3,
        Plasma,
        Missile,
        HomingMissile,
        Bomb,
        Total
    };

    struct Specification {
        enum {
            Destructing = 1 << 0,
            Splitting = 1 << 1,
            Burning = 1 << 2,
            Exploding = 1 << 3,
            Homing = 1 << 4,
        };
        
        sf::IntRect TextureRect;
        sf::IntRect CollisionRect;
        float Damage;
        float Speed;
        float Lifetime;
        uint16_t Flags;
    }; 

    inline constexpr std::array<Specification, static_cast<size_t>(Type::Total)> Specifications = {
        Specification{{{64, 0}, {64, 64}}, {{-4, -8}, {8, 17}}, 1.0f, 10.0f, 1.0f, 
        Specification::Destructing}, // Bullet1
        Specification{{{128, 0}, {64, 64}}, {{-5, -13}, {10, 26}}, 1.0f, 10.0f, 1.0f, 
        Specification::Destructing}, // Bullet2
        Specification{{{192, 0}, {64, 64}}, {{-6, -18}, {12, 36}}, 1.0f, 10.0f, 1.0f, 
        Specification::Destructing}, // Bullet3
        Specification{{{256, 0}, {64, 64}}, {{0, 0}, {64, 64}}, 2.0f, 6.0f, 2.5f, 
        Specification::Destructing | Specification::Splitting}, // Laser1
        Specification{{{320, 0}, {64, 64}}, {{0, 0}, {64, 64}}, 2.0f, 6.0f, 2.5f, 
        Specification::Destructing | Specification::Splitting}, // Laser2
        Specification{{{384, 0}, {64, 64}}, {{0, 0}, {64, 64}}, 2.0f, 6.0f, 2.5f, 
        Specification::Destructing | Specification::Splitting}, // Laser3
        Specification{{{0, 0}, {64, 64}}, {{0, 0}, {64, 64}}, 3.0f, 4.0f, 1.5f, 
        Specification::Burning}, // Plasma
        Specification{{{448, 0}, {64, 64}}, {{0, 0}, {64, 64}}, 4.0f, 6.0f, 3.0f, 
        Specification::Destructing | Specification::Exploding}, // Missile
        Specification{{{512, 0}, {64, 64}}, {{0, 0}, {64, 64}}, 4.0f, 5.0f, 6.0f, 
        Specification::Destructing | Specification::Exploding | Specification::Homing}, // HomingMissile
        Specification{{{576, 0}, {64, 64}}, {{0, 0}, {64, 64}}, 25.0f, 2.5f, 2.0f, 
        Specification::Exploding}  // Bomb
    };

    struct Weapon {
        enum Type {
            Cannon = 0,
            Blaster,
            Burner,
            Launcher,
            SeekerLauncher,
            Dropper,
            Total
        };
        Weapon::Type ThisType;

        Weapon(Weapon::Type type) : ThisType(type) {}

        Projectile::Type GetBulletType(float basePower = 1.0f) {
            switch (ThisType) {
                case Cannon:
                    if (basePower < 5.0f)
                        return Bullet1;
                    if (basePower < 10.0f)
                        return Bullet2;
                    return Bullet3;
                case Blaster:
                    if (basePower < 5.0f)
                        return Laser1;
                    if (basePower < 10.0f)
                        return Laser2;
                    return Laser3;
                case Burner:
                    return Plasma;
                case Launcher:
                    return Missile;
                case SeekerLauncher:
                    return HomingMissile;
                case Dropper:
                    return Bomb;
                default:
                    spdlog::warn("Unknown weapon type, defaulting to Bullet1");
                    return Bullet1;
            }
        }
    };

    struct Component {
        Type ThisType;

        Component(Type type = Type::Bullet1) : ThisType(type) {}

        [[nodiscard]] bool CompareFlags(uint16_t flags) const {
            return (Specifications.at(static_cast<size_t>(ThisType)).Flags & flags) != 0;
        }
    };

    struct Burning {
        float BurnPeriodSeconds = 0.25f;
        std::unordered_map<entt::entity, sf::Clock> BurnClocks;

        bool CanBurn(entt::entity other) {
            return !BurnClocks.contains(other) || 
                    BurnClocks[other].getElapsedTime() >= sf::seconds(BurnPeriodSeconds);
        }
    };

    entt::entity Create(entt::registry &registry, 
        Type type, const sf::Vector2f& position, const sf::Vector2f& direction, 
        entt::entity source);

    void OnCollision(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity);
    float GetProjectileDamage(entt::registry &registry, entt::entity projectileEntity, const Component& component);
    void BurnEntity(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity, float damage, Burning& burningComponent);
}
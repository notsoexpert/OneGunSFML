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

        Projectile::Type getBulletType(float power = 1.0f) {
            switch (ThisType) {
                case Cannon:
                    if (power < 5.0f)
                        return Bullet1;
                    if (power < 10.0f)
                        return Bullet2;
                    return Bullet3;
                case Blaster:
                    if (power < 5.0f)
                        return Laser1;
                    if (power < 10.0f)
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

    struct Burning {
        float BurnPeriodSeconds = 0.25f;
        std::unordered_map<entt::entity, sf::Clock> BurnClocks;

        bool CanBurn(entt::entity other) {
            return !BurnClocks.contains(other) || 
                    BurnClocks[other].getElapsedTime() >= sf::seconds(BurnPeriodSeconds);
        }
    };
    
    entt::entity Create(entt::registry &registry, 
        Type type, const sf::Texture &texture, 
        const sf::Vector2f& position, const sf::Vector2f& direction, 
        entt::entity source);

    void OnCollision(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity);
    
    struct Config {
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

    inline constexpr std::array<Config, static_cast<size_t>(Type::Total)> Presets = {
        Config{ sf::IntRect({64, 0}, {64, 64}), sf::IntRect{{-4, -8}, {8, 17}}, 1.0f, 10.0f, 1.0f, 
        Config::Destructing}, // Bullet1
        Config{ sf::IntRect({128, 0}, {64, 64}), sf::IntRect{{-5, -13}, {10, 26}}, 1.0f, 10.0f, 1.0f, 
        Config::Destructing}, // Bullet2
        Config{ sf::IntRect({192, 0}, {64, 64}), sf::IntRect{{-6, -18}, {12, 36}}, 1.0f, 10.0f, 1.0f, 
        Config::Destructing}, // Bullet3
        Config{ sf::IntRect({256, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 2.0f, 6.0f, 2.5f, 
        Config::Destructing | Config::Splitting}, // Laser1
        Config{ sf::IntRect({320, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 2.0f, 6.0f, 2.5f, 
        Config::Destructing | Config::Splitting}, // Laser2
        Config{ sf::IntRect({384, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 2.0f, 6.0f, 2.5f, 
        Config::Destructing | Config::Splitting}, // Laser3
        Config{ sf::IntRect({448, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 3.0f, 4.0f, 1.5f, 
        Config::Burning}, // Plasma
        Config{ sf::IntRect({576, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 4.0f, 6.0f, 3.0f, 
        Config::Destructing | Config::Exploding}, // Missile
        Config{ sf::IntRect({0, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 4.0f, 5.0f, 6.0f, 
        Config::Destructing | Config::Exploding | Config::Homing}, // HomingMissile
        Config{ sf::IntRect({64, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 25.0f, 2.5f, 2.0f, 
        Config::Exploding}  // Bomb
    };

    struct Component {
        Type ThisType;

        Component(Type type = Type::Bullet1) : ThisType(type) {}

        [[nodiscard]] bool CompareFlags(uint16_t flags) const {
            return (Presets.at(static_cast<size_t>(ThisType)).Flags & flags) != 0;
        }
    };
}
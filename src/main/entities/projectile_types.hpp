#pragma once

#include "entities/projectile.hpp"

namespace Projectile {
    enum Type {
        Bullet = 0,
        Laser,
        Plasma,
        Missile,
        Bomb,
        Ice,
        Total
    };

    enum Flags {
        Destruct = 1 << 0,
        Split = 1 << 1,
        Burn = 1 << 2,
        Explode = 1 << 3,
        Home = 1 << 4
    };

    struct Component {
        Type ThisType;
        size_t Specification;
        float BaseDamage;

        Component(Type type = Type::Bullet, 
            size_t specification = Flags::Destruct, 
            float baseDamage = 1.0f) : 
            ThisType(type), Specification(specification), BaseDamage(baseDamage) {}

        [[nodiscard]] bool CompareFlags(size_t flags) const {
            return (Specification & flags) != 0;
        }
    };

    entt::entity Create(Setup& setup, Type type);
    
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
                    return Bullet;
                case Blaster:
                    return Laser;
                case Burner:
                    return Plasma;
                case Launcher:
                    return Missile;
                case SeekerLauncher:
                    return Missile;
                case Dropper:
                    return Bomb;
                default:
                    spdlog::warn("Unknown weapon type, defaulting projectile to Bullet");
                    return Bullet;
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

    void BurnEntity(entt::registry &registry, entt::entity projectileEntity, entt::entity otherEntity, float damage, Burning& burningComponent);
}
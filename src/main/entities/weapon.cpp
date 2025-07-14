#include "pch.hpp"
#include "weapon.hpp"

namespace Weapon {

    void ChangePreset(Component& weap, Type newType) {
        if (newType < Type::MainCannon || newType >= Type::Total) {
            spdlog::error("ChangePreset: Invalid weapon type: {}", static_cast<uint8_t>(newType));
            return;
        }

        spdlog::warn("Updating weapon preset to {}", static_cast<uint8_t>(newType));
        weap.WeaponType = newType;
        switch (newType) {
            case Type::MainCannon:
                weap.BaseDamage = 1.0f;
                weap.BaseFireRate = 2.0f;
                weap.BaseShotSpeed = 1.0f;
                weap.ProjectileType = Projectile::Type::Bullet;
                weap.ProjectileTier = 2U;
                return;
            case Type::ChainGun:
                weap.BaseDamage = 0.15f;
                weap.BaseFireRate = 6.0f;
                weap.BaseShotSpeed = 1.0f;
                weap.ProjectileType = Projectile::Type::Bullet;
                weap.ProjectileTier = 0U;
                return;
            case Type::Blaster:
                weap.BaseDamage = 1.0f;
                weap.BaseFireRate = 1.5f;
                weap.BaseShotSpeed = 0.75f;
                weap.ProjectileType = Projectile::Type::Laser;
                weap.ProjectileTier = 0U;
                return;
            case Type::PlasmaThrower:
                weap.BaseDamage = 1.0f;
                weap.BaseFireRate = 4.0f;
                weap.BaseShotSpeed = 0.5f;
                weap.ProjectileType = Projectile::Type::Plasma;
                weap.ProjectileTier = 0U;
                return;
            case Type::Launcher:
                weap.BaseDamage = 1.0f;
                weap.BaseFireRate = 1.0f;
                weap.BaseShotSpeed = 1.0f;
                weap.ProjectileType = Projectile::Type::Missile;
                weap.ProjectileTier = 0U;
                return;
            case Type::GuidedLauncher:
                weap.BaseDamage = 1.0f;
                weap.BaseFireRate = 1.0f;
                weap.BaseShotSpeed = 0.8f;
                weap.ProjectileType = Projectile::Type::Missile;
                weap.ProjectileTier = 1U;
                return;
            case Type::Dropper:
                weap.BaseDamage = 1.0f;
                weap.BaseFireRate = 1.0f;
                weap.BaseShotSpeed = 1.0f;
                weap.ProjectileType = Projectile::Type::Bomb;
                weap.ProjectileTier = 0U;
                return;
            case Type::DroneCannon:
                weap.BaseDamage = 1.0f;
                weap.BaseFireRate = 0.5f;
                weap.BaseShotSpeed = 0.5f;
                weap.ProjectileType = Projectile::Type::Bullet;
                weap.ProjectileTier = 0U;
        }
    }

}
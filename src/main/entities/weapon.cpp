#include "pch.hpp"
#include "weapon.hpp"

namespace Weapon {

    void ChangePreset(Component& weap, Type newType) {
        if (newType < Type::MainCannon || newType >= Type::Total) {
            spdlog::error("ChangePreset: Invalid weapon type: {}", static_cast<uint8_t>(newType));
            return;
        }

        spdlog::trace("Updating weapon preset to {}", static_cast<uint8_t>(newType));
        weap.WeaponType = newType;
        switch (newType) {
            case Type::MainCannon:
                weap.CurrentDamageFactor = 1.0f;
                weap.CurrentFireRateFactor = 2.0f;
                weap.CurrentShotSpeedFactor = 1.0f;
                weap.ProjectileType = Projectile::Type::Bullet;
                weap.ProjectileTier = 2U;
                return;
            case Type::ChainGun:
                weap.CurrentDamageFactor = 0.15f;
                weap.CurrentFireRateFactor = 6.0f;
                weap.CurrentShotSpeedFactor = 1.0f;
                weap.ProjectileType = Projectile::Type::Bullet;
                weap.ProjectileTier = 0U;
                return;
            case Type::Blaster:
                weap.CurrentDamageFactor = 1.0f;
                weap.CurrentFireRateFactor = 1.5f;
                weap.CurrentShotSpeedFactor = 0.75f;
                weap.ProjectileType = Projectile::Type::Laser;
                weap.ProjectileTier = 0U;
                return;
            case Type::PlasmaThrower:
                weap.CurrentDamageFactor = 1.0f;
                weap.CurrentFireRateFactor = 4.0f;
                weap.CurrentShotSpeedFactor = 0.5f;
                weap.ProjectileType = Projectile::Type::Plasma;
                weap.ProjectileTier = 0U;
                return;
            case Type::Launcher:
                weap.CurrentDamageFactor = 1.0f;
                weap.CurrentFireRateFactor = 1.0f;
                weap.CurrentShotSpeedFactor = 1.0f;
                weap.ProjectileType = Projectile::Type::Missile;
                weap.ProjectileTier = 0U;
                return;
            case Type::GuidedLauncher:
                weap.CurrentDamageFactor = 1.0f;
                weap.CurrentFireRateFactor = 1.0f;
                weap.CurrentShotSpeedFactor = 0.8f;
                weap.ProjectileType = Projectile::Type::Missile;
                weap.ProjectileTier = 1U;
                return;
            case Type::Dropper:
                weap.CurrentDamageFactor = 1.0f;
                weap.CurrentFireRateFactor = 1.0f;
                weap.CurrentShotSpeedFactor = 1.0f;
                weap.ProjectileType = Projectile::Type::Bomb;
                weap.ProjectileTier = 0U;
                return;
            case Type::DroneCannon:
                weap.CurrentDamageFactor = 1.0f;
                weap.CurrentFireRateFactor = 0.5f;
                weap.CurrentShotSpeedFactor = 0.5f;
                weap.ProjectileType = Projectile::Type::Bullet;
                weap.ProjectileTier = 0U;
        }
    }

}
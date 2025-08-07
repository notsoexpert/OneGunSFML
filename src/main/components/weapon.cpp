#include "pch.hpp"
#include "weapon.hpp"

namespace OneGunGame {

void Weapon::ChangePreset(Weapon::Type newType) {
    if (newType < Type::MainCannon || newType >= Type::Total) {
        spdlog::error("ChangePreset: Invalid weapon type: {}", static_cast<uint8_t>(newType));
        return;
    }

    spdlog::trace("Updating weapon preset to {}", static_cast<uint8_t>(newType));
    WeaponType = newType;
    switch (newType) {
        case Type::MainCannon:
            CurrentDamageFactor = 1.0f;
            CurrentFireRateFactor = 2.0f;
            CurrentShotSpeedFactor = 1.0f;
            ProjectileType = Projectile::Type::Bullet;
            ProjectileTier = 2U;
            return;
        case Type::ChainGun:
            CurrentDamageFactor = 0.15f;
            CurrentFireRateFactor = 6.0f;
            CurrentShotSpeedFactor = 1.0f;
            ProjectileType = Projectile::Type::Bullet;
            ProjectileTier = 0U;
            return;
        case Type::Blaster:
            CurrentDamageFactor = 1.0f;
            CurrentFireRateFactor = 1.5f;
            CurrentShotSpeedFactor = 0.75f;
            ProjectileType = Projectile::Type::Laser;
            ProjectileTier = 0U;
            return;
        case Type::PlasmaThrower:
            CurrentDamageFactor = 1.0f;
            CurrentFireRateFactor = 4.0f;
            CurrentShotSpeedFactor = 0.5f;
            ProjectileType = Projectile::Type::Plasma;
            ProjectileTier = 0U;
            return;
        case Type::Launcher:
            CurrentDamageFactor = 1.0f;
            CurrentFireRateFactor = 1.0f;
            CurrentShotSpeedFactor = 1.0f;
            ProjectileType = Projectile::Type::Missile;
            ProjectileTier = 0U;
            return;
        case Type::GuidedLauncher:
            CurrentDamageFactor = 1.0f;
            CurrentFireRateFactor = 1.0f;
            CurrentShotSpeedFactor = 0.8f;
            ProjectileType = Projectile::Type::Missile;
            ProjectileTier = 1U;
            return;
        case Type::Dropper:
            CurrentDamageFactor = 1.0f;
            CurrentFireRateFactor = 1.0f;
            CurrentShotSpeedFactor = 1.0f;
            ProjectileType = Projectile::Type::Bomb;
            ProjectileTier = 0U;
            return;
        case Type::DroneCannon:
            CurrentDamageFactor = 1.0f;
            CurrentFireRateFactor = 0.5f;
            CurrentShotSpeedFactor = 0.5f;
            ProjectileType = Projectile::Type::Bullet;
            ProjectileTier = 0U;
            return;
        default:
            return;
    }
}

bool Weapon::Fire() {
    if (Clock.getElapsedTime().asSeconds() >= 1.0f / std::clamp(GetFireRate(), 0.0001f, INFINITY)) {
        Clock.restart();
        return true;
    }
    return false;
}

void Weapon::SetBaseStats(float damage, float fireRate, float shotSpeed) {
    BaseDamage = damage;
    BaseFireRate = fireRate;
    BaseShotSpeed = shotSpeed;
    ChangePreset(WeaponType);
}

}
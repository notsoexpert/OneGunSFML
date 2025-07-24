#pragma once

#include "entities/projectile_types.hpp"

namespace Weapon {
    struct Component;

    enum Type : uint8_t {
        MainCannon,
        ChainGun,
        Blaster,
        PlasmaThrower,
        Launcher,
        GuidedLauncher,
        Dropper,
        DroneCannon,
        FighterCannon,
        FighterLauncher,
        BomberDropper,
        HunterCannon,
        HunterLauncher,
        BombardierMainLauncher,
        BombardierClusterDropper,
        BombardierMegaDropper,
        GalaxisBlaster,
        GalaxisMegaDropper,
        GalaxisChainGun,
        GalaxisClusterDropper,
        GalaxisGuidedLauncher,
        GalaxisMegaBlaster,
        Total
    };

    void ChangePreset(Component& weaponComponent, Weapon::Type newType);

    struct Component {
        sf::Vector2f ForwardVector{0.0f, -1.0f};
        float BaseDamage = 1.0f;
        float BaseFireRate = 1.0f;
        float BaseShotSpeed = 1.0f;
        float CurrentDamageFactor = BaseDamage;
        float CurrentFireRateFactor = BaseFireRate;
        float CurrentShotSpeedFactor = BaseShotSpeed;
        uint8_t ProjectileTier = 0U;
        Projectile::Type ProjectileType;
        Weapon::Type WeaponType;

        Component(Weapon::Type type) : WeaponType(type) {
            ChangePreset(*this, type);
        }

        void SetBaseStats(float damage = 1.0f, float fireRate = 1.0f, float shotSpeed = 1.0f) {
            BaseDamage = damage;
            BaseFireRate = fireRate;
            BaseShotSpeed = shotSpeed;
            ChangePreset(*this, WeaponType);
        }

        float GetDamage() const {
            return BaseDamage * CurrentDamageFactor;
        }

        float GetFireRate() const {
            return BaseFireRate * CurrentFireRateFactor;
        }

        float GetShotSpeed() const {
            return BaseShotSpeed * CurrentShotSpeedFactor;
        }
    };
}
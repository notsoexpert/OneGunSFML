#pragma once

#include "entities/projectile.hpp"

namespace Projectile {
    enum class Type {
        Bullet = 0,
        Laser,
        Plasma,
        Missile,
        Bomb,
        Ice,
        Total
    };

    enum Flags : uint8_t {
        Destruct = 1 << 0,
        Split = 1 << 1,
        Burn = 1 << 2,
        Explode = 1 << 3,
        Home = 1 << 4
    };

    struct Component {
        Type ThisType;
        uint8_t Specification;
        float DamageFactor;
        std::optional<uint8_t> Tier{};

        Component(Type type = Type::Bullet, 
            uint8_t specification = Flags::Destruct, 
            float damageFactor = 1.0f) : 
            ThisType(type), 
            Specification(specification), 
            DamageFactor(damageFactor)
        {}

        [[nodiscard]] bool CompareFlags(uint8_t flags) const {
            return (Specification & flags) != 0;
        }
    };

    entt::entity Create(Setup& setup, Type type);
    entt::entity Fire(entt::registry &registry, entt::entity playerEntity);
    
    struct Weapon {
        Projectile::Type ProjectileType;
        std::optional<sf::Vector2f> Direction;
        std::optional<uint8_t> TierOverride;

        Weapon(Projectile::Type type) : ProjectileType(type) {}
        Weapon(Projectile::Type type, const sf::Vector2f& direction) :
            ProjectileType(type), Direction(direction) {}
        Weapon(Projectile::Type type, uint8_t tier) :
            ProjectileType(type), TierOverride(tier) {}
        Weapon(Projectile::Type type, const sf::Vector2f& direction, uint8_t tier) :
            ProjectileType(type), Direction(direction), TierOverride(tier) {}
    };
    
    namespace Bullet {
        size_t GetTier(float basePower);
        void Create(const Setup& setup);
        void Death(entt::registry& registry, entt::entity thisEntity);
    }
    namespace Laser {
        size_t GetTier(float basePower);
        void Create(const Setup& setup);
        void Death(entt::registry& registry, entt::entity thisEntity);
    }
    namespace Plasma {
        void Create(const Setup& setup);
        void Death(entt::registry& registry, entt::entity thisEntity);
    }
    namespace Missile {
        void Create(const Setup& setup);
        void Death(entt::registry& registry, entt::entity thisEntity);
    }
    namespace Bomb {
        void Create(const Setup& setup);
        void Death(entt::registry& registry, entt::entity thisEntity);
    }
    namespace Ice {
        void Create(const Setup& setup);
        void Death(entt::registry& registry, entt::entity thisEntity);
    }
}
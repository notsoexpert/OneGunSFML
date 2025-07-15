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
        uint8_t Tier = 0;

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
    entt::entity Fire(entt::registry &registry, entt::entity playerEntity, std::optional<uint8_t> tierOverride = std::nullopt);
    
    namespace Bullet {
        void Create(const Setup& setup);
        void Death(entt::registry& registry, entt::entity thisEntity);
    }
    namespace Laser {
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
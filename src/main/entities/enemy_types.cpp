#include "pch.hpp"
#include "enemy_types.hpp"

namespace Enemy {
    static const std::array<std::function<void(const Setup&)>, Type::Total> SetupEnemy = {
        AsteroidSetup, LargeAsteroidSetup, HugeAsteroidSetup,
        CometSetup, DroneSetup, FighterSetup, BomberSetup,
        HunterSetup, BombardierSetup, GalaxisSetup
    };

    entt::entity Create(Setup& setup, Type type) {
        if (type < 0 || type >= Enemy::Type::Total) {
            spdlog::error("Create: Invalid enemy type: {}", static_cast<int>(type));
            return entt::null;
        }
        
        setup.ThisEntity = Enemy::Create(setup);
        spdlog::info("Creating Enemy entity {}", static_cast<int>(setup.ThisEntity));
        setup.Registry.emplace<Component>(setup.ThisEntity, type);

        SetupEnemy[type](setup);

        return setup.ThisEntity;
    }
}
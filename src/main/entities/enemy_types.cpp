#include "pch.hpp"
#include "enemy_types.hpp"

namespace OneGunGame{
namespace Enemy {
    static const std::array<std::function<void(const Setup&)>, static_cast<size_t>(Type::Total)> SetupEnemy = {
        Asteroid::Create, LargeAsteroid::Create, HugeAsteroid::Create,
        Comet::Create, Drone::Create, Fighter::Create, Bomber::Create,
        Hunter::Create, Bombardier::Create, Galaxis::Create
    };

    entt::entity Create(Setup& setup, Type type) {
        setup.ThisEntity = Enemy::Create(setup);
        spdlog::info("Creating Enemy entity {}", static_cast<int>(setup.ThisEntity));
        setup.Registry.emplace<Component>(setup.ThisEntity, type);

        SetupEnemy[static_cast<size_t>(type)](setup);

        return setup.ThisEntity;
    }
}
}
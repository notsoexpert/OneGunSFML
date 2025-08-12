#pragma once

#include "entities/enemy.hpp"

namespace OneGunGame{
namespace Enemy {
    enum class Type {
        Asteroid = 0,
        LargeAsteroid,
        HugeAsteroid,
        Comet,
        Drone,
        Fighter,
        Bomber,
        Hunter,
        Bombardier,
        Galaxis,
        Total
    };

    struct Component {
        Type ThisType;

        Component(Type type) : ThisType(type) {}
    };

    entt::entity Create(Entity::Setup& setup, Type type);

    namespace Asteroid{
        void Create(const Entity::Setup& setup);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
    namespace LargeAsteroid{
        void Create(const Entity::Setup& setup);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
    namespace HugeAsteroid{
        void Create(const Entity::Setup& setup);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
    namespace Comet{
        void Create(const Entity::Setup& setup);
        void Behavior(entt::registry &registry, entt::entity thisEntity);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
    namespace Drone {
        void Create(const Entity::Setup& setup);
        void Behavior(entt::registry &registry, entt::entity thisEntity);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
    namespace Fighter {
        void Create(const Entity::Setup& setup);
        void Behavior(entt::registry &registry, entt::entity thisEntity);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
    namespace Bomber {
        void Create(const Entity::Setup& setup);
        void Behavior(entt::registry &registry, entt::entity thisEntity);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
    namespace Hunter {
        void Create(const Entity::Setup& setup);
        void Behavior(entt::registry &registry, entt::entity thisEntity);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
    namespace Bombardier {
        void Create(const Entity::Setup& setup);
        void Behavior(entt::registry &registry, entt::entity thisEntity);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
    namespace Galaxis {
        void Create(const Entity::Setup& setup);
        void Behavior(entt::registry &registry, entt::entity thisEntity);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
}
}
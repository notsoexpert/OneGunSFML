#pragma once

#include "entities/enemy.hpp"

namespace Enemy {
    enum Type {
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

    entt::entity Create(Setup& setup, Type type);

    void AsteroidSetup(const Setup& setup);
    void LargeAsteroidSetup(const Setup& setup);
    void HugeAsteroidSetup(const Setup& setup);
    void CometSetup(const Setup& setup);
    void DroneSetup(const Setup& setup);
    void FighterSetup(const Setup& setup);
    void BomberSetup(const Setup& setup);
    void HunterSetup(const Setup& setup);
    void BombardierSetup(const Setup& setup);
    void GalaxisSetup(const Setup& setup);
    void CometBehavior(entt::registry &registry, entt::entity thisEntity);
    void DroneBehavior(entt::registry &registry, entt::entity thisEntity);
    void FighterBehavior(entt::registry &registry, entt::entity thisEntity);
    void BomberBehavior(entt::registry &registry, entt::entity thisEntity);
    void HunterBehavior(entt::registry &registry, entt::entity thisEntity);
    void BombardierBehavior(entt::registry &registry, entt::entity thisEntity);
    void GalaxisBehavior(entt::registry &registry, entt::entity thisEntity);
    void AsteroidDeath(entt::registry &registry, entt::entity thisEntity);
    void LargeAsteroidDeath(entt::registry &registry, entt::entity thisEntity);
    void HugeAsteroidDeath(entt::registry &registry, entt::entity thisEntity);
    void CometDeath(entt::registry &registry, entt::entity thisEntity);
    void DroneDeath(entt::registry &registry, entt::entity thisEntity);
    void FighterDeath(entt::registry &registry, entt::entity thisEntity);
    void BomberDeath(entt::registry &registry, entt::entity thisEntity);
    void HunterDeath(entt::registry &registry, entt::entity thisEntity);
    void BombardierDeath(entt::registry &registry, entt::entity thisEntity);
    void GalaxisDeath(entt::registry &registry, entt::entity thisEntity);
}
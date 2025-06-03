#pragma once

#include <SFML/Graphics/Rect.hpp>

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

    constexpr std::array<const char*, Total> Names {
        "Asteroid", "Large Asteroid", "Huge Asteroid",
        "Comet", "Drone", "Fighter", "Bomber", "Hunter",
        "Bombardier", "Galaxis"
    };

    void Update(entt::registry &registry);

    entt::entity Create(entt::registry &registry, 
        Type type, const sf::Vector2f& position, const sf::Vector2f& direction,
        entt::entity source = entt::null);

    entt::entity Fire(entt::registry &registry, entt::entity sourceEntity);

    void OnCollision(entt::registry &registry, entt::entity thisEntity, entt::entity otherEntity);

    void RemoveOffscreenLifetime(entt::registry &registry, entt::entity thisEntity);
    void AddOffscreenLifetime(entt::registry &registry, entt::entity thisEntity);

    void DroneBehavior(entt::registry &registry, entt::entity thisEntity);
    void CometBehavior(entt::registry &registry, entt::entity thisEntity);
    void FighterBehavior(entt::registry &registry, entt::entity thisEntity);
    void BomberBehavior(entt::registry &registry, entt::entity thisEntity);
    void HunterBehavior(entt::registry &registry, entt::entity thisEntity);
    void BombardierBehavior(entt::registry &registry, entt::entity thisEntity);
    void GalaxisBehavior(entt::registry &registry, entt::entity thisEntity);
    
    struct Component {
        Type ThisType;

        Component(Type type) : ThisType(type) {}
    };

    struct Behavior {
        using Action = std::function<void(entt::registry &registry, entt::entity thisEntity)>;
        Action Act;

        Behavior(Action act) : Act(act) {}

        static const std::unordered_map<Enemy::Type, Action> Callbacks;
    };

    struct Data {
        sf::IntRect TextureRect;
        sf::IntRect CollisionRect;
        float MoveSpeed;
        float OffscreenLifetime;
        float MaxHealth;
        float FireRate = 1.0f;
    }; 
    
    constexpr std::array<Data, Type::Total> Presets = {
        Data{ sf::IntRect({128, 64}, {64, 64}), sf::IntRect{{-8, -8}, {16, 16}}, 3.0f, 5.0f, 10.0f},  // Asteroid
        Data{ sf::IntRect({192, 64}, {64, 64}), sf::IntRect{{-23, -23}, {46, 46}}, 2.0f, 5.0f, 25.0f}, // LargeAsteroid
        Data{ sf::IntRect({256, 64}, {64, 64}), sf::IntRect{{-32, -32}, {64, 64}}, 1.0f, 5.0f, 50.0f}, // HugeAsteroid
        Data{ sf::IntRect({256, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f, 100.0f},  // Comet
        Data{ sf::IntRect({320, 64}, {64, 64}), sf::IntRect{{-24, -24}, {48, 48}}, 2.0f, 5.0f, 20.0f, 0.5f},  // Drone
        Data{ sf::IntRect({384, 64}, {64, 64}), sf::IntRect{{-24, -24}, {48, 48}}, 5.0f, 5.0f, 40.0f},  // Fighter
        Data{ sf::IntRect({448, 64}, {64, 64}), sf::IntRect{{-24, -24}, {48, 48}}, 3.0f, 5.0f, 50.0f},  // Bomber
        Data{ sf::IntRect({576, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 3.0f, 150.0f},  // Hunter
        Data{ sf::IntRect({0, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 5.0f, 6.0f, 300.0f},   // Bombardier
        Data{ sf::IntRect({64, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 2.5f, 2.0f, 1000.0f}   // Galaxis
    };
    
    
}
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

    entt::entity Create(entt::registry &registry, 
        Type type, const sf::Texture &texture, 
        const sf::Vector2f& position, const sf::Vector2f& direction,
        entt::entity source = entt::null);

    void OnCollision(entt::registry &registry, entt::entity thisEntity, entt::entity otherEntity);

    void RemoveOffscreenLifetime(entt::registry &registry, entt::entity thisEntity);
    void AddOffscreenLifetime(entt::registry &registry, entt::entity thisEntity);

    void AsteroidBehavior(entt::registry &registry, entt::entity thisEntity);
    void CometBehavior(entt::registry &registry, entt::entity thisEntity);
    void DroneBehavior(entt::registry &registry, entt::entity thisEntity);
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
    }; 
    
    constexpr std::array<Data, Type::Total> Presets = {
        Data{ sf::IntRect({128, 64}, {64, 64}), sf::IntRect{{24, 24}, {16, 16}}, 3.0f, 5.0f, 10.0f},  // Asteroid
        Data{ sf::IntRect({192, 64}, {64, 64}), sf::IntRect{{9, 9}, {46, 46}}, 2.0f, 5.0f, 25.0f}, // LargeAsteroid
        Data{ sf::IntRect({256, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 1.0f, 5.0f, 50.0f}, // HugeAsteroid
        Data{ sf::IntRect({256, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f, 100.0f},  // Comet
        Data{ sf::IntRect({320, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f, 20.0f},  // Drone
        Data{ sf::IntRect({384, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 2.5f, 40.0f},  // Fighter
        Data{ sf::IntRect({448, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 4.0f, 1.5f, 50.0f},  // Bomber
        Data{ sf::IntRect({576, 0}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 6.0f, 3.0f, 150.0f},  // Hunter
        Data{ sf::IntRect({0, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 5.0f, 6.0f, 300.0f},   // Bombardier
        Data{ sf::IntRect({64, 64}, {64, 64}), sf::IntRect{{0, 0}, {64, 64}}, 2.5f, 2.0f, 1000.0f}   // Galaxis
    };
    
    
}
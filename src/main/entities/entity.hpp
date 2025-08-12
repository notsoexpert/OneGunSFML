#pragma once

#include "systems/constants.hpp"

namespace OneGunGame {
struct Dashable;
struct Velocity;

namespace Entity {

    struct Setup {
        entt::registry &Registry;
        sf::Vector2f Position;
        sf::Vector2f Direction;
        std::optional<CollisionLayer> CLayer;
        std::optional<uint8_t> CMask;
        std::optional<uint8_t> Tier;
        entt::entity Source = entt::null;
        entt::entity ThisEntity = entt::null;
    };

    void Update(entt::registry &registry);

    float GetBasePower(entt::registry &registry, entt::entity thisEntity);

    bool Damage(entt::registry &registry, entt::entity attacker, entt::entity target, float damage);

    void SetupOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, float duration = 1.0f);
    void RemoveOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, std::variant<int, float> duration);
    void AddOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, std::variant<int, float> duration);
    
    void StartDash(Dashable& dashable, Velocity& velocity);
}
}
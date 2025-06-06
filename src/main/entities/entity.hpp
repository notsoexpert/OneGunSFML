#pragma once

namespace Entity {

    void Update(entt::registry &registry);

    bool Damage(entt::registry &registry, entt::entity attacker, entt::entity target, float damage);

    void RemoveOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, std::variant<int, float> duration);
    void AddOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, std::variant<int, float> duration);
}
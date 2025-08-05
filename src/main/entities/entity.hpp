#pragma once

namespace OneGunGame {
namespace Entity {

    void Update(entt::registry &registry);

    float GetBasePower(entt::registry &registry, entt::entity thisEntity);

    bool Damage(entt::registry &registry, entt::entity attacker, entt::entity target, float damage);

    void SetupOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, float duration = 1.0f);
    void RemoveOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, std::variant<int, float> duration);
    void AddOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, std::variant<int, float> duration);
}
}
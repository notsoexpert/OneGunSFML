#include "pch.hpp"
#include "entity.hpp"

#include "system/components.hpp"

namespace Entity {

    bool Damage(entt::registry &registry, entt::entity attacker, entt::entity target, float damage) {
        auto otherHealth = registry.try_get<Health>(target);
        if (!otherHealth) {
            spdlog::error("Entity {} hit by entity {} is missing health component", 
            static_cast<int>(target), static_cast<int>(attacker));
            return false;
        }
        
        spdlog::info("Entity {} damaged. New Health: {}", static_cast<int>(target), otherHealth->Current);
        otherHealth->Damage(damage);
        return true;
    }
}
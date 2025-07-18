#include "pch.hpp"
#include "entity.hpp"

#include "system/onegungame.hpp"
#include "system/components.hpp"

#include "entities/projectile_types.hpp"
#include "entities/weapon.hpp"

namespace Entity {

    void Update(entt::registry& registry) {
        registry.view<Renderable, ScreenTrigger>().each(
            [&](auto entity, Renderable &renderable, ScreenTrigger &screenTrigger) {
                sf::FloatRect screenRect{sf::Vector2f{}, static_cast<sf::Vector2f>(OneGunGame::GetWindowSize())};
                sf::Vector2f spriteSize = static_cast<sf::Vector2f>(renderable.Sprite.getTextureRect().size);
                sf::FloatRect spriteRect{renderable.Sprite.getPosition(), spriteSize};
                
                if (spriteRect.findIntersection(screenRect)) {
                    screenTrigger.Enter(registry, entity, screenTrigger.Arg);
                } else {
                    screenTrigger.Leave(registry, entity, screenTrigger.Arg);
                }
        });
    }

    float GetBasePower(entt::registry &registry, entt::entity thisEntity) {
        auto weapComp = registry.try_get<Weapon::Component>(thisEntity);
        if (!weapComp){
            spdlog::warn("GetBasePower called on entity {} with no Weapon component, returning 1.0f", static_cast<int>(thisEntity));
            return 1.0f;
        }
        return weapComp->BaseDamage;
    }

    bool Damage(entt::registry &registry, entt::entity attacker, entt::entity target, float damage) {
        auto otherHealth = registry.try_get<Health>(target);
        if (!otherHealth) {
            spdlog::error("Entity {} hit by entity {} is missing health component", 
            static_cast<int>(target), static_cast<int>(attacker));
            return false;
        }
        
        otherHealth->Damage(damage);
        spdlog::info("Entity {} damaged by {}. New Health: {}", static_cast<int>(target), damage, otherHealth->Current);
        if (otherHealth->IsDead()) {
            registry.emplace_or_replace<Dying>(target, otherHealth->OnDeath);
        }
        return true;
    }

    void SetupOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, float expireTimeInSeconds){
        registry.emplace<ScreenTrigger>(thisEntity, Entity::RemoveOffscreenLifetime, Entity::AddOffscreenLifetime, expireTimeInSeconds);
    }

    void RemoveOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, [[maybe_unused]]std::variant<int, float> duration) {
        if (registry.remove<Lifetime>(thisEntity) > 0)
            spdlog::trace("Lifetime component removed from {}", static_cast<int>(thisEntity));
    }

    void AddOffscreenLifetime(entt::registry &registry, entt::entity thisEntity, std::variant<int, float> durationInSeconds) {
        auto lifetime = registry.try_get<Lifetime>(thisEntity);
        float duration = 1.0f;
        try {
            duration = std::get<float>(durationInSeconds);
        } catch (const std::bad_variant_access& e) {
            spdlog::warn("{}; Non-float Arg passed to AddOffscreenLifetime", e.what());
        }
        if (!lifetime) {
            registry.emplace<Lifetime>(thisEntity, sf::seconds(duration));
            spdlog::trace("Lifetime component added to {}", static_cast<int>(thisEntity));
        }
    }
}
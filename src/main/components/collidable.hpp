#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "systems/constants.hpp"
#include <variant>

namespace OneGunGame {
struct Collision {
    entt::registry& Registry;
    entt::entity ThisEntity;
    entt::entity OtherEntity;
};

struct Collidable {
    using Func = std::function<void(Collision& collision)>;
    
    sf::IntRect CollisionRect;
    entt::entity Source;
    OneGunGame::CollisionLayer CLayer;
    uint8_t CMask;

    Func OnCollision;

    Collidable(const sf::IntRect& rect, entt::entity source, OneGunGame::CollisionLayer layer, 
        uint8_t mask, Func onCollision = {}) : 
        CollisionRect(rect),
        Source(source),
        CLayer(layer),
        CMask(mask),
        OnCollision(onCollision)
    {}
};

struct HitLimiting {
    std::unordered_set<entt::entity> HitEntities;
};

struct ScreenTrigger {
    using Func = std::function<void(entt::registry&, entt::entity, std::variant<int, float>)>;

    Func Enter;
    Func Leave;
    std::variant<int, float> Arg;

    ScreenTrigger(Func enter, Func leave, int iArg) : Enter(enter), Leave(leave), Arg(iArg) {}
    ScreenTrigger(Func enter, Func leave, float fArg) : Enter(enter), Leave(leave), Arg(fArg) {}
};
}
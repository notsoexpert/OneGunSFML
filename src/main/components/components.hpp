#pragma once

#include <functional>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "system/constants.hpp"

struct Health {
    float Max;
    float Current;

    Health(float max) :
        Max(max),
        Current(max)
    {}
};

struct Energy {
    float Max;
    float Current;

    Energy(float max) :
        Max(max),
        Current(max)
    {}
};

struct Speed {
    float MoveMaxSpeed;
    float MoveAcceleration;

    Speed(float moveMaxSpeed = 5.0f, float moveAcceleration = 0.5f) :
        MoveMaxSpeed(moveMaxSpeed),
        MoveAcceleration(moveAcceleration)
    {}
};

struct HitInvincibility {
    sf::Clock InvincibilityClock;
    float BaseInvincibleTime;

    HitInvincibility(float baseInvincibleTime = 1.0f) :
        BaseInvincibleTime(baseInvincibleTime)
    {}

    bool IsInvincible() const {
        return InvincibilityClock.getElapsedTime().asSeconds() <= BaseInvincibleTime;
    }

};

struct Fireable {
    float BaseDamage;
    float BaseFireRate;
    sf::Clock FireClock;

    Fireable(float baseDamage = 1.0f, float baseFireRate = 1.0f) :
        BaseDamage(baseDamage),
        BaseFireRate(baseFireRate)
    {}

    bool Fire(float weaponRate = 0.1f) {
        if (FireClock.getElapsedTime().asSeconds() >= weaponRate * BaseFireRate) {
            FireClock.restart();
            return true;
        }
        return false;
    }
};

struct Dashable {
    sf::Clock DashClock;
    sf::Clock DashCooldownClock;
    sf::Vector2f m_LastDirection;
    float BaseDashSpeedMultiplier = 3.0f;
    float BaseDashDuration = 0.1f;
    float BaseDashCooldown = 1.0f;

    Dashable(float baseDashSpeedMultiplier = 3.0f, float baseDashDuration = 0.1f, float baseDashCooldown = 1.0f) :
        BaseDashSpeedMultiplier(baseDashSpeedMultiplier),
        BaseDashDuration(baseDashDuration),
        BaseDashCooldown(baseDashCooldown)
    {
        DashClock.restart();
        DashCooldownClock.restart();
    }

    bool IsDashComplete() {
        return DashClock.getElapsedTime().asSeconds() >= BaseDashDuration;
    }

    bool IsDashCooldownComplete() {
        return DashCooldownClock.getElapsedTime().asSeconds() >= BaseDashCooldown;
    }

    void Dash() {
        if (IsDashCooldownComplete()){
            DashClock.restart();
            DashCooldownClock.restart();
        }
    }
};

struct Renderable {
    sf::Sprite Sprite;
    int DrawOrder;
    Renderable(const sf::Texture& texture, int drawOrder = 0) : 
        Sprite(texture), 
        DrawOrder(drawOrder) 
    {}
};

struct Collidable {
    sf::IntRect CollisionRect;
    entt::entity Source;
    OneGunGame::CollisionLayer Layer;
    OneGunGame::CollisionLayer Mask;

    std::function<void(entt::registry& registry, entt::entity thisEntity, entt::entity otherEntity)> OnCollision;

    Collidable(const sf::IntRect& rect, entt::entity source, OneGunGame::CollisionLayer layer, 
        OneGunGame::CollisionLayer mask, auto onCollision = [](entt::registry&, entt::entity, entt::entity) {}) : 
        CollisionRect(rect),
        Source(source),
        Layer(layer),
        Mask(mask),
        OnCollision(onCollision)
    {}
};

struct Collision {
    entt::entity EntityA;
    entt::entity EntityB;

    Collision(entt::entity a, entt::entity b) : 
        EntityA(a),
        EntityB(b)
    {}
};
struct Lifetime {
    sf::Time Duration;
    sf::Clock Clock;
    Lifetime(sf::Time duration) : 
        Duration(duration), 
        Clock() 
    {}
};

using Velocity = sf::Vector2f;
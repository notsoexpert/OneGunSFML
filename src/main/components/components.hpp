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

struct MaxSpeed {
    float Value;

    MaxSpeed(float value = 5.0f) :
        Value(value)
    {}
};

struct HitInvincibility {
    sf::Clock Clock;
    float Duration;

    HitInvincibility(float duration = 1.0f) :
        Duration(duration)
    {}

    bool IsInvincible() const {
        return Clock.getElapsedTime().asSeconds() <= Duration;
    }

};

struct Fireable {
    float BaseDamage;
    float BaseFireRate;
    sf::Clock Clock;

    Fireable(float baseDamage = 1.0f, float baseFireRate = 1.0f) :
        BaseDamage(baseDamage),
        BaseFireRate(baseFireRate)
    {}

    bool Fire(float weaponRate = 0.1f) {
        if (Clock.getElapsedTime().asSeconds() >= weaponRate * BaseFireRate) {
            Clock.restart();
            return true;
        }
        return false;
    }
};

struct Dashable {
    sf::Clock DashClock;
    sf::Clock DashCooldownClock;
    sf::Vector2f LastDirection;
    float SpeedMultiplier = 3.0f;
    float Duration = 0.1f;
    float Cooldown = 1.0f;

    Dashable(float speedMultiplier = 3.0f, float duration = 0.1f, float cooldown = 1.0f) :
        SpeedMultiplier(speedMultiplier),
        Duration(duration),
        Cooldown(cooldown)
    {
        DashClock.restart();
        DashCooldownClock.restart();
    }

    bool IsDashComplete() {
        return DashClock.getElapsedTime().asSeconds() >= Duration;
    }

    bool IsDashCooldownComplete() {
        return DashCooldownClock.getElapsedTime().asSeconds() >= Cooldown;
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

struct Confined {
    sf::FloatRect Limits;

    Confined(const sf::FloatRect& limits) :
        Limits(limits)
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

struct Friction {
    float Value;

    Friction(float value = 0.1f) :
        Value(value)
    {}
};

struct Velocity {
    sf::Vector2f Value;

    Velocity(float x = 0.0f, float y = 0.0f) :
        Value(x, y)
    {}
    Velocity(const sf::Vector2f& vec) :
        Value(vec)
    {}
    operator sf::Vector2f() const {
        return Value;
    }
    operator sf::Vector2f&() {
        return Value;
    }
};

struct Acceleration {
    sf::Vector2f Value;

    Acceleration(float x = 0.0f, float y = 0.0f) :
        Value(x, y)
    {}
    Acceleration(const sf::Vector2f& vec) :
        Value(vec)
    {}
    operator sf::Vector2f() const {
        return Value;
    }
    operator sf::Vector2f&() {
        return Value;
    }
};
#pragma once

#include <functional>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <entt/entt.hpp>

#include "system/constants.hpp"

struct Health {
    using Func = std::function<void(entt::registry& registry, entt::entity thisEntity)>;
    float Max;
    float Current;

    Func OnDeath;
    
    Health(float max, Func onDeath = {}) :
        Max(max),
        Current(max),
        OnDeath(onDeath)
    {}

    void Damage(float amount) { 
        Current = std::clamp(Current - amount, 0.0f, Max);
    }

    void Heal(float amount) {
        Damage(-amount);
    }

    bool IsDead() const {
        return Current <= 0.0f;
    }
};

struct Energy {
    float Max;
    float Current;

    Energy(float max) :
        Max(max),
        Current(max)
    {}

    void Exert(float amount) { 
        Current = std::clamp(Current - amount, 0.0f, Max);
    }

    void Restore(float amount) {
        Exert(-amount);
    }
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

    bool Fire(float weaponRate = 1.0f) {
        if (Clock.getElapsedTime().asSeconds() >= 1.0f / std::clamp(weaponRate * BaseFireRate, 0.001f, INFINITY)) {
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
#if DEBUG
    sf::RectangleShape DebugRect;
#endif

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
    using Func = std::function<void(entt::registry& registry, entt::entity thisEntity, entt::entity otherEntity)>;
    
    sf::IntRect CollisionRect;
    entt::entity Source;
    OneGunGame::CollisionLayer Layer;
    OneGunGame::CollisionLayer Mask;

    Func OnCollision;

    Collidable(const sf::IntRect& rect, entt::entity source, OneGunGame::CollisionLayer layer, 
        OneGunGame::CollisionLayer mask, Func onCollision = {}) : 
        CollisionRect(rect),
        Source(source),
        Layer(layer),
        Mask(mask),
        OnCollision(onCollision)
    {}
};

struct ScreenTrigger {
    using Func = std::function<void(entt::registry&, entt::entity, std::variant<int, float>)>;

    Func Enter;
    Func Leave;
    std::variant<int, float> Arg;

    ScreenTrigger(Func enter, Func leave, int iArg) : Enter(enter), Leave(leave), Arg(iArg) {}
    ScreenTrigger(Func enter, Func leave, float fArg) : Enter(enter), Leave(leave), Arg(fArg) {}
};
struct Lifetime {
    sf::Time Duration;
    sf::Clock Clock;
    Lifetime(sf::Time duration) : 
        Duration(duration)
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

struct Rotating {
    sf::Angle RotationsPerSecond;

    Rotating(sf::Angle rotationsPerSecond) : RotationsPerSecond(rotationsPerSecond) {}
};

struct Destructing {};
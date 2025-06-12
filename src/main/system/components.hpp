#pragma once

#include <functional>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <entt/entt.hpp>

#include "system/constants.hpp"

using Callback = std::function<void(entt::registry& registry, entt::entity thisEntity)>;

struct Health { 
    float Max;
    float Current;

    Callback OnDeath;
    
    Health(float max, Callback onDeath = {}) :
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

struct Dying {
    Callback OnDeath;

    Dying(Callback onDeath = {}) : 
        OnDeath(onDeath)
    {}
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

struct Animation {
    sf::Vector2i TextureRectStartPosition;
    size_t CurrentFrame = 0;
    size_t TotalFrames;
    sf::Clock AnimationClock;
    sf::Time FrameTime;

    Animation(const sf::Vector2i& texRectStartPos, size_t totalFrames, sf::Time frameTime) :
        TextureRectStartPosition(texRectStartPos),
        TotalFrames(totalFrames),
        FrameTime(frameTime)
    {
        AnimationClock.restart();
    }

    bool IsFrameComplete() {
        if (AnimationClock.getElapsedTime() >= FrameTime){
            AnimationClock.restart();
            return true;
        }
        return false;
    }
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

struct Scaling {
    sf::Clock ScaleClock;
    sf::Time ScaleTime;
    sf::Vector2f OriginalScale;
    sf::Vector2f TargetScale;

    Scaling(const sf::Vector2f& originalScale, const sf::Vector2f& targetScale, sf::Time scaleTime) :
        OriginalScale(originalScale),
        TargetScale(targetScale),
        ScaleTime(scaleTime)
    {
        ScaleClock.restart();
    }

    bool IsScaleComplete() {
        return ScaleClock.getElapsedTime() >= ScaleTime;
    }

    float GetScalePercentage() {
        if (ScaleTime.asSeconds() == 0.0f){
            return 1.0f;
        }

        return ScaleClock.getElapsedTime().asSeconds() / ScaleTime.asSeconds();
    }
};

struct Fading {
    sf::Clock FadeClock;
    sf::Time FadeTime;
    uint8_t OriginalAlpha;
    uint8_t TargetAlpha;

    Fading(uint8_t originalAlpha, uint8_t targetAlpha, sf::Time fadeTime) :
        OriginalAlpha(originalAlpha),
        TargetAlpha(targetAlpha),
        FadeTime(fadeTime)
    {
        FadeClock.restart();
    }

    bool IsFadeComplete() {
        return FadeClock.getElapsedTime() >= FadeTime;
    }

    float GetFadePercentage() {
        if (FadeTime.asSeconds() == 0.0f){
            return 1.0f;
        }

        return FadeClock.getElapsedTime().asSeconds() / FadeTime.asSeconds();
    }
};

struct Destructing {};
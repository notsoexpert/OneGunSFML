#pragma once
#include <entt/entt.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

namespace OneGunGame {
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

struct Dashable {
    enum class DashState : uint8_t {
        Starting = 0,
        Accelerating,
        Decelerating,
        Ending,
        None
    };
    static constexpr auto AccelerationDashDurationFactor = 0.25f;
    static constexpr auto DecelerationDashDurationFactor = 1.0f - AccelerationDashDurationFactor;
    sf::Clock DashClock;
    sf::Clock DashCooldownClock;
    sf::Vector2f LastDirection;
    float SpeedMultiplier, Duration, Cooldown;
    DashState CurrentState = DashState::None;

    Dashable(float speedMultiplier = 5.0f, float duration = 0.5f, float cooldownInSeconds = 1.0f) :
        SpeedMultiplier(speedMultiplier),
        Duration(duration),
        Cooldown(cooldownInSeconds)
    {}

    bool IsDashComplete() {
        return DashClock.getElapsedTime().asSeconds() >= Duration;
    }

    bool IsDashCooldownComplete() {
        return DashCooldownClock.getElapsedTime().asSeconds() >= Cooldown;
    }

    bool Dash(const sf::Vector2f& direction) {
        if (CurrentState == DashState::None && IsDashCooldownComplete()){
            LastDirection = direction;
            CurrentState = DashState::Starting;
            return true;
        }
        return false;
    }
};
}
#pragma once
#include <entt/entt.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

#include "utils/math.hpp"

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
        Progressing,
        Ending,
        None
    };
    BezierCurve<float, 3> AccelerationCurve;
    sf::Clock DashClock;
    sf::Clock DashCooldownClock;
    sf::Vector2f LastDirection;
    float SpeedMultiplier, Duration, Cooldown;
    DashState CurrentState = DashState::None;

    Dashable(const std::array<sf::Vector2f, 3>& curveControlPoints, 
        float speedMultiplier = 5.0f, float duration = 0.5f, float cooldownInSeconds = 1.0f) :
        AccelerationCurve(curveControlPoints),
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
};
}
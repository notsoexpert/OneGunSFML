#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

namespace OneGunGame {
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
struct MaxSpeed {
    float Value;

    MaxSpeed(float value = 5.0f) :
        Value(value)
    {}
};

struct Confined {
    sf::FloatRect Limits;

    Confined(const sf::FloatRect& limits) :
        Limits(limits)
    {}
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
        ScaleTime(scaleTime),
        OriginalScale(originalScale),
        TargetScale(targetScale)
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
}
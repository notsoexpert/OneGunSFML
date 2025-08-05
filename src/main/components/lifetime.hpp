#pragma once
#include <entt/entt.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

#include "systems/constants.hpp"

namespace OneGunGame {

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

struct Lifetime {
    sf::Time Duration;
    sf::Clock Clock;
    Callback OnDeath;

    Lifetime(sf::Time duration, Callback onDeath = {}) : 
        Duration(duration),
        OnDeath(onDeath)
    {}
};

struct Attachment {
    entt::entity Parent;
    Callback OnDetach;

    Attachment(entt::entity parent, Callback onDetach = {}) :
        Parent(parent),
        OnDetach(onDetach)
    {}
};

struct Destructing {};
}
#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

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
    Collidable(const sf::IntRect& rect, entt::entity source = entt::null) : 
        CollisionRect(rect),
        Source(source) 
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
#include "pch.hpp"
#include "background.hpp"

#include "system/onegungame.hpp"
#include "components/components.hpp"

namespace Background {

entt::entity Create(entt::registry &registry, const sf::Texture &texture) {
    entt::entity entity = registry.create();
    entt::entity copy = registry.create();

    auto &renderable = registry.emplace<Renderable>(entity, texture, Background::Order);
    auto &copyRenderable = registry.emplace<Renderable>(copy, texture, Background::Order);
    renderable.Sprite.setPosition(StartPosition);
    copyRenderable.Sprite.setPosition(StartPosition);
    copyRenderable.Sprite.move(sf::Vector2f{0.0f, 0.0f - texture.getSize().y});
    
    registry.emplace<HorizontalParallax>(entity);
    registry.emplace<HorizontalParallax>(copy);

    sf::Vector2f fWindowSize = static_cast<sf::Vector2f>(OneGunGame::GetWindowSize());
    registry.emplace<Confined>(entity, 
        sf::FloatRect{-fWindowSize, 
            sf::Vector2f{fWindowSize.x, fWindowSize.y * 2}});
    registry.emplace<Confined>(copy, 
        sf::FloatRect{sf::Vector2f{-fWindowSize.x, -fWindowSize.y - texture.getSize().y},
            sf::Vector2f{fWindowSize.x, fWindowSize.y * 4}});

    registry.emplace<Velocity>(entity, sf::Vector2f(0.0f, ScrollSpeed));
    registry.emplace<Velocity>(copy, sf::Vector2f(0.0f, ScrollSpeed));

    registry.emplace<VerticalLoop>(entity, fWindowSize.y, -fWindowSize.y, copy);
    return entity;
}

}
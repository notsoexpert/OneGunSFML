#include "pch.hpp"
#include "background.hpp"

#include "systems/onegungame.hpp"
#include "systems/components.hpp"

namespace Background {

void Update(entt::registry &registry) {
    registry.view<Renderable, Velocity, Background::VerticalLoop>().each(
        [&](auto entity, Renderable &renderable, Velocity &velocity, Background::VerticalLoop &vLoop) {
        spdlog::trace("Updating entity {}: vLoop LimitY: {}, ResetY: {}, Copy Entity: {}", 
            static_cast<int>(entity), vLoop.LimitY, vLoop.ResetY, static_cast<int>(vLoop.BackgroundCopy));
        auto pos = renderable.Sprite.getPosition();
        const auto &vel = velocity.Value;
        
        if (pos.y + vel.y <= vLoop.LimitY) 
            return;
        
        auto &copyRenderable = registry.get<Renderable>(vLoop.BackgroundCopy);
        renderable.Sprite.setPosition({pos.x, vLoop.ResetY});
        copyRenderable.Sprite.setPosition({pos.x, vLoop.ResetY - copyRenderable.Sprite.getTexture().getSize().y});
    });
    // TODO: Use DeltaTime
    registry.view<Renderable, Background::HorizontalParallax>().each(
        [&](auto entity, Renderable& renderable, Background::HorizontalParallax& parallax) {
        spdlog::trace("Updating entity {}: Parallax value ({})", static_cast<int>(entity), parallax.Value);
        const auto &sourcePosition = registry.get<Renderable>(parallax.SourceEntity).Sprite.getPosition();
        parallax.Value = sourcePosition.x / OneGunGame::GetWindowSize().x;
        renderable.Sprite.setPosition({OneGunGame::GetWindowSize().x * -parallax.Value, renderable.Sprite.getPosition().y});
    });
}

entt::entity Create(entt::registry &registry) {
    entt::entity entity = registry.create();
    entt::entity copy = registry.create();

    auto &renderable = registry.emplace<Renderable>(entity, OneGunGame::GetTexture(OneGunGame::Background), Background::Order);
    auto &copyRenderable = registry.emplace<Renderable>(copy, OneGunGame::GetTexture(OneGunGame::Background), Background::Order);
    renderable.Sprite.setPosition(StartPosition);
    copyRenderable.Sprite.setPosition(StartPosition);
    copyRenderable.Sprite.move(sf::Vector2f{0.0f, 0.0f - OneGunGame::GetTexture(OneGunGame::Background).getSize().y});
    
    registry.emplace<HorizontalParallax>(entity, OneGunGame::GetPlayerEntity());
    registry.emplace<HorizontalParallax>(copy, OneGunGame::GetPlayerEntity());

    sf::Vector2f fWindowSize = static_cast<sf::Vector2f>(OneGunGame::GetWindowSize());
    registry.emplace<Confined>(entity, 
        sf::FloatRect{-fWindowSize, 
            sf::Vector2f{fWindowSize.x, fWindowSize.y * 2}});
    registry.emplace<Confined>(copy, 
        sf::FloatRect{sf::Vector2f{-fWindowSize.x, -fWindowSize.y - OneGunGame::GetTexture(OneGunGame::Background).getSize().y},
            sf::Vector2f{fWindowSize.x, fWindowSize.y * 4}});

    registry.emplace<Velocity>(entity, sf::Vector2f(0.0f, ScrollSpeed));
    registry.emplace<Velocity>(copy, sf::Vector2f(0.0f, ScrollSpeed));

    registry.emplace<VerticalLoop>(entity, fWindowSize.y, -fWindowSize.y, copy);
    return entity;
}

}
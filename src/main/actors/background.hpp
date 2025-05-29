#pragma once

namespace Background {
    constexpr sf::Vector2f StartPosition{0.0f, -600.0f};
    constexpr auto Order = 100;
    constexpr auto ScrollSpeed = 5.0f;

    struct HorizontalParallax {
        float Value = 0.0f;
    };

    struct VerticalLoop {
        float LimitY;
        float ResetY;
        entt::entity BackgroundCopy;

        VerticalLoop(float limitY = 0.0f, float resetY = 0.0f, entt::entity backgroundCopy = entt::null) :
            LimitY(limitY),
            ResetY(resetY),
            BackgroundCopy(backgroundCopy)
        {}
    };

    entt::entity Create(entt::registry &registry, const sf::Texture &texture);
}
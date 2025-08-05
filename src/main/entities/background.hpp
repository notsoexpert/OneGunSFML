#pragma once

namespace OneGunGame{
namespace Background {
    constexpr sf::Vector2f StartPosition{0.0f, -600.0f};
    constexpr auto Order = 100;
    constexpr auto ScrollSpeed = 5.0f;

    struct HorizontalParallax {
        entt::entity SourceEntity;
        float Value = 0.0f;

        HorizontalParallax(entt::entity sourceEntity) : SourceEntity(sourceEntity) {}
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

    void Update(entt::registry &registry);
    entt::entity Create(entt::registry &registry);
}
}
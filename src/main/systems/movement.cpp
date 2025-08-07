#include "pch.hpp"
#include "onegungame.hpp"
#include "utils/math.hpp"

#include "components/renderable.hpp"
#include "components/transformation.hpp"
#include "components/mechanics.hpp"

namespace OneGunGame
{

void ProcessMovement(/*DeltaTime dt*/) {
    // TODO: Use DeltaTime
    static std::unordered_set<entt::entity> dashers;
    GetData().Registry.view<Dashable, Acceleration>().each(
        [](auto entity, Dashable& dashable, Acceleration& acceleration)
        {
            switch (dashable.CurrentState) {
                case Dashable::DashState::Starting:
                    spdlog::warn("Dash Starting");
                    dashers.insert(entity);
                    dashable.DashClock.restart(); 
                    acceleration.Value = 
                        dashable.LastDirection * 
                        dashable.SpeedMultiplier * 
                        GetQuadraticCurvePoint<float>(dashable.AccelerationCurve, 0.0f).y;

                    dashable.CurrentState = Dashable::DashState::Progressing;
                    return;
                case Dashable::DashState::Progressing:
                {
                    spdlog::warn("Dash Progressing");
                    dashers.insert(entity);

                    auto dashPercentage = std::clamp(dashable.DashClock.getElapsedTime().asSeconds() / dashable.Duration, 0.0f, 1.0f);
                    acceleration.Value = 
                        dashable.LastDirection * 
                        dashable.SpeedMultiplier * 
                        GetQuadraticCurvePoint<float>(
                            dashable.AccelerationCurve, 
                            dashPercentage
                        ).y;
                    if (dashPercentage >= 1.0f) {
                        dashable.CurrentState = Dashable::DashState::Ending;
                    }
                    return;
                }
                case Dashable::DashState::Ending:
                    spdlog::warn("Dash Ending");
                    dashable.DashCooldownClock.restart();
                    dashable.CurrentState = Dashable::DashState::None;
                    return;
                default:
                    return;
            }
        }
    );

    GetData().Registry.view<Acceleration, Velocity>().each(
        [](auto entity, Acceleration &acceleration, Velocity &velocity)
        {
            spdlog::trace("Updating entity {}: Velocity ({}, {}) with Acceleration ({}, {})",
                          static_cast<int>(entity), velocity.Value.x, velocity.Value.y, acceleration.Value.x, acceleration.Value.y);
            velocity.Value += acceleration.Value;
            if (std::abs(velocity.Value.x) < 0.01f)
                velocity.Value.x = 0.0f;
            if (std::abs(velocity.Value.y) < 0.01f)
                velocity.Value.y = 0.0f;
        });

    // TODO: Use DeltaTime
    GetData().Registry.view<Friction, Velocity>().each(
        [](auto Entity, Friction &friction, Velocity &velocity)
        {
            spdlog::trace("Applying friction to entity {}: Velocity ({}, {}) with Friction ({})",
                          static_cast<int>(Entity), velocity.Value.x, velocity.Value.y, friction.Value);
            velocity.Value *= (1.0f - friction.Value);
            if (std::abs(velocity.Value.x) < 0.01f)
                velocity.Value.x = 0.0f;
            if (std::abs(velocity.Value.y) < 0.01f)
                velocity.Value.y = 0.0f;
        });

    GetData().Registry.view<MaxSpeed, Velocity>().each(
        [](auto entity, MaxSpeed &maxSpeed, Velocity &velocity){
            if (dashers.contains(entity)) {
                return; // Dashers uncapped
            }
            spdlog::trace("Clamping speed for entity {}: Velocity ({}, {}) with MaxSpeed ({})", 
                static_cast<int>(entity), velocity.Value.x, velocity.Value.y, maxSpeed.Value);

            if (velocity.Value.length() <= maxSpeed.Value) return;
            velocity.Value = velocity.Value.normalized() * maxSpeed.Value; 
        });
    dashers.clear();
}

void ApplyMovement() {
    // TODO: Use DeltaTime
    GetData().Registry.view<Renderable, Velocity>().each(
        [](auto entity, Renderable& renderable, Velocity& velocity) {
        spdlog::trace("Updating entity {}: Position ({}, {}), Velocity ({}, {})", 
            static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y, velocity.Value.x, velocity.Value.y);
        renderable.Sprite.move(velocity.Value);
    });

    GetData().Registry.view<Renderable, Confined>().each(
        [&](auto entity, Renderable &renderable, Confined &confined) {
        spdlog::trace("Updating entity {}: Confined to ({}, {}, {}, {})", static_cast<int>(entity), 
            confined.Limits.position.x, confined.Limits.position.y, confined.Limits.size.x, confined.Limits.size.y);
        auto position = renderable.Sprite.getPosition();
        position.x = std::clamp(position.x, confined.Limits.position.x, confined.Limits.position.x + confined.Limits.size.x);
        position.y = std::clamp(position.y, confined.Limits.position.y, confined.Limits.position.y + confined.Limits.size.y);
        renderable.Sprite.setPosition(position);
    });
}
}
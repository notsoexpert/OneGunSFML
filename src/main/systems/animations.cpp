#include "pch.hpp"
#include "onegungame.hpp"

#include "utils/math.hpp"

#include "components/renderable.hpp"
#include "components/transformation.hpp"
#include "components/animation.hpp"

namespace OneGunGame{

void UpdateAnimations() {
    GetData().Registry.view<Renderable, Animation>().each(
        [](Renderable& renderable, Animation& animation) {
            if (!animation.IsFrameComplete()) {
                return;
            }

            auto texRect = renderable.Sprite.getTextureRect();
            animation.SetFrame(ConstrainValueLooped<int>(static_cast<int>(animation.CurrentFrame) + animation.AnimationDirection, 0, static_cast<int>(animation.TotalFrames)), texRect);
            renderable.Sprite.setTextureRect(texRect);
        }
    );

    GetData().Registry.view<Renderable, Scaling>().each(
        [](auto entity, Renderable& renderable, Scaling& scaling) {
            if (scaling.IsScaleComplete()) {
                renderable.Sprite.setScale(scaling.TargetScale);
                GetData().Registry.remove<Scaling>(entity);
                return;
            }
            float scalePercentage = scaling.GetScalePercentage();
            spdlog::trace("Scale percent: {}", scalePercentage);
            sf::Vector2f scaleFactor = LinearInterpolation(scaling.OriginalScale, scaling.TargetScale, scalePercentage);
            renderable.Sprite.setScale(scaleFactor);
        }
    );

    GetData().Registry.view<Renderable, Fading>().each(
        [](Renderable& renderable, Fading& fading) {
            auto color = renderable.Sprite.getColor();
            if (fading.IsFadeComplete()) {
                color.a = fading.TargetAlpha;
                renderable.Sprite.setColor(color);
                return;
            }
            float fadePercentage = fading.GetFadePercentage();
            spdlog::trace("Fade percent: {}", fadePercentage);
            color.a = LinearInterpolation(fading.OriginalAlpha, fading.TargetAlpha, fadePercentage);
            renderable.Sprite.setColor(color);
        }
    );

    // TODO: Use DeltaTime
    GetData().Registry.view<Renderable, Rotating>().each(
        [](auto entity, Renderable &renderable, Rotating &rotating) {
            spdlog::trace("Updating entity {}: Rotating {} radians per second", 
            static_cast<int>(entity), rotating.RotationsPerSecond.asRadians());
            renderable.Sprite.rotate(rotating.RotationsPerSecond / static_cast<float>(Configuration.FrameRateLimit));
        }
    );
}
}
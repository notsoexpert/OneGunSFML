#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace OneGunGame {
struct Animation {
    sf::Vector2i TextureRectStartPosition;
    size_t CurrentFrame = 0;
    size_t TotalFrames;
    sf::Clock AnimationClock;
    sf::Time FrameTime;
    int AnimationDirection;

    Animation(const sf::Vector2i& texRectStartPos, size_t totalFrames, sf::Time frameTime, int animationDirection = 1) :
        TextureRectStartPosition(texRectStartPos),
        TotalFrames(totalFrames),
        FrameTime(frameTime),
        AnimationDirection(animationDirection)
    {
        AnimationClock.restart();
    }

    bool IsFrameComplete() {
        if (AnimationClock.getElapsedTime() >= FrameTime){
            AnimationClock.restart();
            return true;
        }
        return false;
    }

    void SetFrame(size_t frame, sf::IntRect& rTexRect) {
        CurrentFrame = frame;
        rTexRect.position = TextureRectStartPosition + rTexRect.size.componentWiseMul(sf::Vector2i{static_cast<int>(CurrentFrame), 0});
    }
};

struct Fading {
    sf::Clock FadeClock;
    sf::Time FadeTime;
    uint8_t OriginalAlpha;
    uint8_t TargetAlpha;

    Fading(uint8_t originalAlpha, uint8_t targetAlpha, sf::Time fadeTime) :
        FadeTime(fadeTime),
        OriginalAlpha(originalAlpha),
        TargetAlpha(targetAlpha)
    {
        FadeClock.restart();
    }

    bool IsFadeComplete() {
        return FadeClock.getElapsedTime() >= FadeTime;
    }

    float GetFadePercentage() {
        if (FadeTime.asSeconds() == 0.0f){
            return 1.0f;
        }

        return FadeClock.getElapsedTime().asSeconds() / FadeTime.asSeconds();
    }
};
}
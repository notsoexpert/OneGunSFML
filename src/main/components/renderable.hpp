#pragma once
#include <SFML/Graphics/Sprite.hpp>

namespace OneGunGame {
struct Renderable {
    sf::Sprite Sprite;
#if DEBUG
    sf::RectangleShape DebugRect;
#endif

    int DrawOrder;
    Renderable(const sf::Texture& texture, int drawOrder = 0) : 
        Sprite(texture), 
        DrawOrder(drawOrder) 
    {}
};
}
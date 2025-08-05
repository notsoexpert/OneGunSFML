#include "pch.hpp"
#include "onegungame.hpp"

#include "components/renderable.hpp"

namespace OneGunGame{
void Sort() {
    GetData().Registry.sort<Renderable>(
        [](const Renderable& a, const Renderable& b) {
            return a.DrawOrder > b.DrawOrder;
        }
    );
}
void Render() {
    GetData().Context.Window.clear(sf::Color::Black);

    GetData().Registry.view<Renderable>().each(
        [](auto entity, Renderable& renderable) {
        spdlog::trace("Rendering entity {}: Position ({}, {})", 
            static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y);
        GetData().Context.Window.draw(renderable.Sprite);
#if DEBUGN
        GetData().Context.Window.draw(renderable.DebugRect);
#endif
    });
    
    GetData().Context.Window.display();
}
}
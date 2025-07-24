#include "pch.hpp"
#include "onegungame.hpp"

#include "systems/components.hpp"

namespace OneGunGame{
sf::Vector2f GetAdjustedCollisionRectSize(const sf::IntRect& originalRect, const sf::Vector2f& renderableScale) {
    return {originalRect.size.x * renderableScale.x, originalRect.size.y * renderableScale.y};
}

sf::IntRect GetAdjustedCollisionRect(const sf::IntRect& originalRect, const sf::Vector2f& renderablePosition, const sf::Vector2f& renderableScale) {
    auto newSize = GetAdjustedCollisionRectSize(originalRect, renderableScale);
    return sf::IntRect{RoundVector(static_cast<sf::Vector2f>(originalRect.position) + renderablePosition - (newSize / 2.0f)), RoundVector(newSize)};
}

bool FilterCollidable(uint8_t mask, OneGunGame::CollisionLayer type) {
    return (mask & static_cast<uint8_t>(type)) != 0;
}

void CheckCollisions() {
    std::unordered_map<entt::entity, std::unordered_set<entt::entity>> processedCollisions;
    GetData().Registry.view<Renderable, Collidable>().each(
        [&](auto entity, Renderable &renderable, Collidable& collidable) {
            spdlog::trace("Checking collisions for entity {}: CollisionRect ({}, {}, {}, {})", 
                static_cast<int>(entity), collidable.CollisionRect.position.x, 
                collidable.CollisionRect.position.y, collidable.CollisionRect.size.x, 
                collidable.CollisionRect.size.y);
            auto rectA = GetAdjustedCollisionRect(collidable.CollisionRect, 
                renderable.Sprite.getPosition(), renderable.Sprite.getScale());

#if DEBUG
            renderable.DebugRect.setSize(static_cast<sf::Vector2f>(rectA.size));
            renderable.DebugRect.setOutlineColor(sf::Color::Red);
            renderable.DebugRect.setOutlineThickness(1.0f);
            renderable.DebugRect.setFillColor(sf::Color::Transparent);
            renderable.DebugRect.setPosition(static_cast<sf::Vector2f>(rectA.position));
#endif

            processedCollisions[entity] = {};
            for (auto [otherEntity, otherRenderable, otherCollidable] : GetData().Registry.view<Renderable, Collidable>().each()) {
                if (GetData().Registry.any_of<Destructing, Dying>(entity)) {
                    return;
                }
                processedCollisions[entity].insert(otherEntity);
                if (GetData().Registry.any_of<Destructing, Dying>(otherEntity)) {
                    continue;
                }
                if (processedCollisions.contains(otherEntity) && processedCollisions[otherEntity].contains(entity)) {
                    continue;
                }
                if (entity == otherEntity || otherEntity == collidable.Source) {
                    continue;
                }
                if (!FilterCollidable(collidable.CollisionMask, otherCollidable.CollisionLayer)) {
                    continue;
                }

                auto rectB = GetAdjustedCollisionRect(otherCollidable.CollisionRect, 
                    otherRenderable.Sprite.getPosition(), otherRenderable.Sprite.getScale());
                if (rectA.findIntersection(rectB)) {
                    spdlog::trace("Collision detected between entity {} and entity {}", 
                        static_cast<int>(entity), static_cast<int>(otherEntity));
                    auto forwardCollision = Collision{GetData().Registry, entity, otherEntity};
                    auto reverseCollision = Collision{GetData().Registry, otherEntity, entity};
                    collidable.OnCollision(forwardCollision);
                    otherCollidable.OnCollision(reverseCollision);
                }
            }
        }
    );
}
}
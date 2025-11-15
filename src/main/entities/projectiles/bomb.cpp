#include "pch.hpp"
#include "entities/projectile_types.hpp"

#include "system/onegungame.hpp"
#include "entities/entity.hpp"
#include "system/components.hpp"
#include "entities/explosion_types.hpp"

namespace Projectile::Bomb {
    static constexpr const char* Name = "Bomb";
    static constexpr OneGunGame::Images ImageID = OneGunGame::Images::SpriteSheet;
    static constexpr sf::IntRect TextureRect = {{576, 0}, {64, 64}};
    static constexpr float MoveSpeed = 2.5f;
    static constexpr float LifeTimeInSeconds = 2.0f;
    static constexpr float BoomDelayInSeconds = 0.45f;
    static constexpr uint8_t Specification = Flags::Explode;

    void Create(const Setup& setup) {
        spdlog::trace("Setting up {} at ({}, {})", Name, setup.Position.x, setup.Position.y);
        
        SetupRenderable(setup, ImageID, TextureRect);
        setup.Registry.emplace<Velocity>(setup.ThisEntity, setup.Direction * MoveSpeed);
        setup.Registry.emplace<Component>(setup.ThisEntity, Projectile::Type::Bomb, Specification, 0.0f);
        setup.Registry.emplace<Lifetime>(setup.ThisEntity, sf::seconds(LifeTimeInSeconds), Death);
    }

    void Death(entt::registry &registry, entt::entity thisEntity) {
        registry.emplace<Destructing>(thisEntity);

        auto pos = registry.get<Renderable>(thisEntity).Sprite.getPosition();

        Explosion::Setup explosionSetup{
            registry,
            pos,
            registry.get<Velocity>(thisEntity).Value,
            {},
            {}
        };
        Explosion::Prebomb::Create(explosionSetup);

        auto boomTickerEntity = registry.create();
        auto& renderable = registry.emplace<Renderable>(boomTickerEntity, OneGunGame::GetTexture(OneGunGame::Images::Unknown), 1000);
        renderable.Sprite.setPosition(pos);
        registry.emplace<Lifetime>(boomTickerEntity, sf::seconds(BoomDelayInSeconds), Explosion::Bomb::CreateBoomWave);
    }

}
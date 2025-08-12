#pragma once

#include "systems/constants.hpp"
#include "entities/explosion.hpp"

namespace OneGunGame{
struct Renderable;

namespace Explosion {
    enum class Type {
        VisualOnly = 0,
        Missile,
        Bomb,
        Total
    };

    struct Component {
        Type ThisType;
        float BaseDamage;

        Component(Type type, float baseDamage = 0.0f) : ThisType(type), BaseDamage(baseDamage) {}
    };

    entt::entity Create(Entity::Setup& setup, Type type);

    namespace BulletHit {
        void Create(Entity::Setup& setup);
    }
    namespace LaserHit {
        void Create(Entity::Setup& setup);
        void SetImageIndex(Renderable& renderable, size_t tier);
    }
    namespace BurnHit {
        void Create(Entity::Setup& setup);
    }
    namespace Missile {
        void Create(Entity::Setup& setup);
    }
    namespace Prebomb {
        void Create(Entity::Setup& setup);
        void Death(entt::registry &registry, entt::entity thisEntity);
    }
    namespace Bomb {
        void Create(Entity::Setup& setup);
        void CreateBoomWave(entt::registry &registry, entt::entity boomTicker);
    }
    namespace IceHit {
        void Create(Entity::Setup& setup);
    }
    namespace PlayerDeath {
        void Create(Entity::Setup& setup);
    }
    namespace AsteroidDeath {
        void Create(Entity::Setup& setup);
    }
    namespace CometDeath {
        void Create(Entity::Setup& setup);
    }
    namespace DroneDeath {
        void Create(Entity::Setup& setup);
    }
    namespace FighterDeath {
        void Create(Entity::Setup& setup);
    }
    namespace BomberDeath {
        void Create(Entity::Setup& setup);
    }
    namespace HunterDeath {
        void Create(Entity::Setup& setup);
    }
    namespace BombardierDeath {
        void Create(Entity::Setup& setup);
    }
    namespace GalaxisDeath {
        void Create(Entity::Setup& setup);
    }
}
}
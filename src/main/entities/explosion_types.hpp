#pragma once

#include "system/constants.hpp"
#include "entities/explosion.hpp"

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

        Component(Type type) : ThisType(type) {}
    };

    entt::entity Create(Setup& setup, Type type);

    namespace BulletHit {
        void Create(Setup& setup);
    }
    namespace LaserHit {
        void Create(Setup& setup);
        void SetImageWithTier(Renderable& renderable, size_t tier);
    }
    namespace BurnHit {
        void Create(Setup& setup);
    }
    namespace Missile {
        void Create(Setup& setup);
    }
    namespace Bomb {
        void Create(Setup& setup);
    }
    namespace IceHit {
        void Create(Setup& setup);
    }
    namespace PlayerDeath {
        void Create(Setup& setup);
    }
    namespace AsteroidDeath {
        void Create(Setup& setup);
    }
    namespace CometDeath {
        void Create(Setup& setup);
    }
    namespace DroneDeath {
        void Create(Setup& setup);
    }
    namespace FighterDeath {
        void Create(Setup& setup);
    }
    namespace BomberDeath {
        void Create(Setup& setup);
    }
    namespace HunterDeath {
        void Create(Setup& setup);
    }
    namespace BombardierDeath {
        void Create(Setup& setup);
    }
    namespace GalaxisDeath {
        void Create(Setup& setup);
    }
}
#pragma once

#include "system/constants.hpp"
#include "entities/explosion.hpp"

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

    namespace VisualOnly {
        namespace BulletHit {
            void Create(Setup& setup);
        }
    }
}
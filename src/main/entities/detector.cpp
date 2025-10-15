#include "pch.hpp"
#include "detector.hpp"

#include "entities/entity.hpp"

namespace OneGunGame {
namespace Detector {

    entt::entity Create(Entity::Setup &setup)
    {
        setup.ThisEntity = setup.Registry.create();
    
        return setup.ThisEntity;
    }
}
}
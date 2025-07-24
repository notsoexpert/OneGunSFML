#include "pch.hpp"
#include "onegungame.hpp"

#include "systems/components.hpp"

namespace OneGunGame{
void ProcessDeaths(){
    GetData().Registry.view<Attachment>().each(
        [](auto entity, Attachment& attachment) {
            if (!GetData().Registry.valid(attachment.Parent)) {
                GetData().Registry.emplace_or_replace<Dying>(entity, attachment.OnDetach);
                GetData().Registry.remove<Attachment>(entity);
                return;
            }
            if (GetData().Registry.any_of<Destructing, Dying>(attachment.Parent)) {
                GetData().Registry.emplace_or_replace<Dying>(entity, attachment.OnDetach);
                GetData().Registry.remove<Attachment>(entity);
                return;
            }
        }
    );

    GetData().Registry.view<Lifetime>(entt::exclude<Destructing, Dying>).each(
        [](auto entity, Lifetime& lifetime) {
            if (lifetime.Clock.getElapsedTime() >= lifetime.Duration) {
                spdlog::trace("Destructing entity {} due to lifetime expiration", static_cast<int>(entity));
                if (lifetime.OnDeath) {
                    GetData().Registry.emplace_or_replace<Dying>(entity, lifetime.OnDeath);
                    return;
                }
                GetData().Registry.emplace_or_replace<Destructing>(entity);
            }
        }
    );

    GetData().Registry.view<Dying>(entt::exclude<Destructing>).each(
        [](auto entity, Dying& dying) {
            if (dying.OnDeath) {
                dying.OnDeath(GetData().Registry, entity);
            }
            GetData().Registry.remove<Dying>(entity);
        }
    );
}

void RecycleEntities(){
    GetData().Registry.view<Destructing>().each(
        [](auto entity) {
            GetData().Registry.destroy(entity);
        }
    );
}
}
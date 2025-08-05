#include "pch.hpp"
#include "onegungame.hpp"

#include "entities/player.hpp"
#include "entities/enemy_types.hpp"
#include "entities/background.hpp"

#include "components/weapon.hpp"

namespace OneGunGame{
void SetupContext() {
    GetData().Context.Window = sf::RenderWindow(sf::VideoMode(Default::WindowSize), WindowTitle, WindowStyle, Configuration.WindowState, Configuration.ContextSettings);
    GetData().Context.Window.setFramerateLimit(Configuration.FrameRateLimit);
    GetData().Context.View.setCenter(sf::Vector2f{ViewSize.x / 2, ViewSize.y / 2});
    GetData().Context.View.setSize(static_cast<sf::Vector2f>(ViewSize));

    GetData().Context.Window.setView(GetData().Context.View);
}

std::expected<void, std::string> SetupTextures() {
    if (!LoadTexture(Images::BG, true, true)) {
        return std::unexpected<std::string>("Failed to load background texture");
    }
    if (!LoadTexture(Images::SpriteSheet, true)) {
        return std::unexpected<std::string>("Failed to load sprite sheet texture");
    }
    if (!LoadTexture(Images::ExplosionRed, true)) {
        return std::unexpected<std::string>("Failed to load red explosion texture");
    }
    if (!LoadTexture(Images::ExplosionYellow, true)) {
        return std::unexpected<std::string>("Failed to load yellow explosion texture");
    }
    if (!LoadTexture(Images::ExplosionGreen, true)) {
        return std::unexpected<std::string>("Failed to load green explosion texture");
    }
    if (!LoadTexture(Images::ExplosionBlue, true)) {
        return std::unexpected<std::string>("Failed to load blue explosion texture");
    }
    if (!LoadTexture(Images::ExplosionViolet, true)) {
        return std::unexpected<std::string>("Failed to load violet explosion texture");
    }
    if (!GeneratePlaceholderTexture()) {
        return std::unexpected<std::string>("Failed to generate placeholder texture");
    }
    return {};
}

bool PausePressed(auto keyCode) {
    return keyCode == sf::Keyboard::Key::Escape;
}
bool ChangeWeaponPressed(auto keyCode) {
    return keyCode == sf::Keyboard::Key::Q;
}

void SetupEventHandlers() {
    GetData().EventHandlers.OnClose = [](const sf::Event::Closed&) {
            GetData().Context.Window.close();
    };
    GetData().EventHandlers.KeyPressed = [](const sf::Event::KeyPressed& keyEvent) {
        if (PausePressed(keyEvent.code)) {
            GetData().Context.Window.close();
        }
        if (ChangeWeaponPressed(keyEvent.code)) {
            auto &weapon = GetData().Registry.get<Weapon>(GetData().Entities.Player);
            weapon.ChangePreset(static_cast<Weapon::Type>((static_cast<uint8_t>(weapon.WeaponType) + 1U) % static_cast<uint8_t>(Weapon::Type::DroneCannon + 1U)));
        }
#ifdef DEBUG
        if (keyEvent.code == sf::Keyboard::Key::T) {
            spdlog::error("TOTAL ENTITIES: {}", GetEntityCount());
        }

        if (keyEvent.code == sf::Keyboard::Key::N) {
            sf::Vector2f spawnPosition = {GetData().Random.generateFloat(-50.0f, GetWindowSize().x + 50.0f),
                                        GetData().Random.generateFloat(-96.0f, -32.0f)};
            sf::Vector2f flyDirection = {0.0f, 1.0f};
            flyDirection = flyDirection.rotatedBy(sf::radians(GetData().Random.generateFloat(-HalfPi, HalfPi)));
            
            Enemy::Setup setup{
                GetData().Registry, 
                spawnPosition, 
                flyDirection, 
                CollisionLayer::Enemy,
                GetCollisionMask({
                    CollisionLayer::Player, CollisionLayer::PlayerProjectile,
                    CollisionLayer::NeutralProjectile}
                ),
                entt::null, 
                entt::null
            };
            Enemy::Create(setup, static_cast<Enemy::Type>(GetData().Random.generateInt(static_cast<int>(Enemy::Type::Asteroid), static_cast<int>(Enemy::Type::Drone))));
        }
#endif
    };
}

void SetupRegistry() {
    GetData().Registry.clear();
}
void SetupLevel() {
    GetData().Entities.Background = Background::Create(GetData().Registry);
    GetData().Entities.Player = Player::Create(GetData().Registry, Player::Start(GetWindowSize()));
}

bool LoadTexture(Images imgType, bool smooth, bool repeat) {
    if (GetData().Textures[imgType].Load()) {
        GetData().Textures[imgType].Texture.setSmooth(smooth);
        GetData().Textures[imgType].Texture.setRepeated(repeat);
        return true;
    }
    return false;
}

bool GeneratePlaceholderTexture() {
    sf::Image img{{2u, 2u}, sf::Color::Magenta};
    return GetData().Textures[Images::Unknown].Texture.loadFromImage(img);
}
}
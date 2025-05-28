#include "pch.hpp"
#include "onegungame.hpp"

#include "system/random.hpp"
#include "components/components.hpp"

namespace OneGunGame {

    struct {
        sf::RenderWindow Window;
    } static Context;

    struct {
        sf::Texture BackgroundTexture;
        sf::Texture SpriteSheetTexture;
    } static s_Textures;

    struct {
        std::function<void(const sf::Event::Closed&)> OnClose;
        std::function<void(const sf::Event::KeyPressed&)> KeyPressed;
    } static s_EventHandlers;

    struct {
        entt::entity Background;
        entt::entity Player;
        std::vector<entt::entity> Projectiles;
        std::vector<entt::entity> Enemies;
    } static s_Entities;

    static entt::registry s_Registry;
    static Random s_Random = Random(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

    int Start(int argc, char *argv[]) {
        spdlog::info("spdlog Version: {0}.{1}.{2}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
        spdlog::info("SFML Version: {0}.{1}.{2}", SFML_VERSION_MAJOR, SFML_VERSION_MINOR, SFML_VERSION_PATCH);
        spdlog::info("EnTT Version: {0}.{1}.{2}", ENTT_VERSION_MAJOR, ENTT_VERSION_MINOR, ENTT_VERSION_PATCH);

        spdlog::info("Argc: {0}", argc);
        for (int i = 0; i < argc; ++i) {
            spdlog::info("Argv[{0}]: {1}", i, argv[i]);
        }

        spdlog::info("Setting up OneGunGame...");
        auto setupResult = Setup(/*pass command line arguments for specific window config*/);
        if (!setupResult.has_value()) {
            spdlog::error("Setup failed: {}", setupResult.error());
            return -1;
        }
        spdlog::info("Setup completed successfully.");

        spdlog::info("Running OneGunGame...");
        auto runResult = Run();
        if (!runResult.has_value()) {
            spdlog::error("Run failed: {}", runResult.error());
            return -1;
        }
        spdlog::info("Run completed successfully.");

        spdlog::info("Cleaning up OneGunGame...");
        auto cleanupResult = Cleanup();
        if (!cleanupResult.has_value()) {
            spdlog::error("Cleanup failed: {}", cleanupResult.error());
            return -1;
        }
        spdlog::info("Cleanup completed successfully.");

        return 0;
    }

    std::expected<int, std::string> Setup() {
        Context.Window = {sf::VideoMode(DefaultWindowSize), WindowTitle, DefaultWindowStyle, DefaultWindowState, DefaultContextSettings};
        Context.Window.setFramerateLimit(DefaultFrameRateLimit);

        if (!s_Textures.BackgroundTexture.loadFromFile("./assets/textures/bg1.png")) {
            return std::unexpected<std::string>("Failed to load background texture");
        }
        s_Textures.BackgroundTexture.setSmooth(true);
        if (!s_Textures.SpriteSheetTexture.loadFromFile("./assets/textures/sprites.png")) {
            return std::unexpected<std::string>("Failed to load sprite sheet texture");
        }
        s_Textures.SpriteSheetTexture.setSmooth(true);

        s_EventHandlers.OnClose = [](const sf::Event::Closed&) {
                Context.Window.close();
        };
        s_EventHandlers.KeyPressed = [](const sf::Event::KeyPressed& keyEvent) {
            if (keyEvent.code == sf::Keyboard::Key::Escape) {
                Context.Window.close();
            }
        };
        
        s_Registry.clear();

        s_Entities.Background = CreateBackground();
        s_Entities.Player = CreatePlayer();

        return 0;
    }

    std::expected<int, std::string> Run() {
        while (Context.Window.isOpen()) {
            Update();
            s_Registry.sort<Renderable>([](const Renderable& a, const Renderable& b) {
                return a.DrawOrder > b.DrawOrder;
            });
            Render();
        }
        return 0;
    }

    std::expected<int, std::string> Cleanup() {
        return 0;
    }

    void Update() {
        Context.Window.handleEvents(s_EventHandlers.OnClose, s_EventHandlers.KeyPressed);

        sf::Vector2f inputVector = GetInputVector();
        s_Registry.get<Velocity>(s_Entities.Player) = inputVector * Player::DefaultMoveSpeed;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            // auto& playerData = s_Registry.get<Player::Data>(s_Entities.Player);
            // if (playerData.m_CanDash && playerData.m_DashClock.getElapsedTime().asSeconds() >= playerData.m_DashCooldown) {
            //     playerData.m_CanDash = false;
            //     playerData.m_DashClock.restart();
            //     s_Registry.get<Velocity>(s_Entities.Player) *= playerData.m_DashSpeed;
            //     playerData.m_DashClock.restart();
            // }
            // DASH
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
            auto &playerData = s_Registry.get<Player::Data>(s_Entities.Player);
            if (playerData.m_FireClock.getElapsedTime().asSeconds() >= playerData.m_FireRate) {
                playerData.m_FireClock.restart();
                spdlog::trace("Player firing projectile");
                auto projectileType = static_cast<Projectile::Type>(s_Random.GenerateInt(0, static_cast<int>(Projectile::Type::Total) - 1));
                spdlog::trace("Projectile type: {}", static_cast<int>(projectileType));
                auto projectile = CreateProjectile(projectileType, s_Registry.get<Renderable>(s_Entities.Player).Sprite.getPosition(), sf::Vector2f{0.0f, -1.0f}, s_Entities.Player);
                s_Entities.Projectiles.push_back(projectile);
            }
        }
        
        s_Registry.view<Renderable, Velocity>().each([](auto entity, Renderable& renderable, Velocity& velocity) {
            spdlog::trace("Updating entity {}: Position ({}, {}), Velocity ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y, velocity.x, velocity.y);
            renderable.Sprite.move(velocity);
        });

        s_Registry.view<Lifetime>().each([](auto entity, Lifetime& lifetime) {
            if (lifetime.Clock.getElapsedTime() >= lifetime.Duration) {
                spdlog::trace("Removing entity {} due to lifetime expiration", static_cast<int>(entity));
                if (std::find(s_Entities.Projectiles.begin(), s_Entities.Projectiles.end(), entity) != s_Entities.Projectiles.end()) {
                    s_Entities.Projectiles.erase(std::remove(s_Entities.Projectiles.begin(), s_Entities.Projectiles.end(), entity));
                }
                s_Registry.destroy(entity);
            }
        });
    }

    void Render() {
        Context.Window.clear(sf::Color::Black);

        s_Registry.view<Renderable>().each([](auto entity, Renderable& renderable) {
            spdlog::trace("Rendering entity {}: Position ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y);
            Context.Window.draw(renderable.Sprite);
        });
        
        Context.Window.display();
    }

    entt::entity CreateBackground() {
        entt::entity entity = s_Registry.create();
        s_Registry.emplace<Renderable>(entity, s_Textures.BackgroundTexture, 100);
        return entity;
    }

    entt::entity CreatePlayer() {
        entt::entity entity = s_Registry.create();
        auto& playerSprite = s_Registry.emplace<Renderable>(entity, s_Textures.SpriteSheetTexture, 10).Sprite;
        playerSprite.setTextureRect({{0, 0}, Player::Size});
        playerSprite.setOrigin({Player::Size.x / 2.0f, Player::Size.y / 2.0f});
        playerSprite.setPosition(Player::Start);
        s_Registry.emplace<Velocity>(entity, 0.0f, 0.0f);
        s_Registry.emplace<Collidable>(entity, sf::IntRect{Player::CollisionOffset, Player::CollisionSize});
        s_Registry.emplace<Player::Data>(entity);
        return entity;
    }
    
    entt::entity CreateProjectile(Projectile::Type type, const sf::Vector2f& position, const sf::Vector2f& direction, entt::entity source) {
        entt::entity entity = s_Registry.create();
        auto &projectileSprite = s_Registry.emplace<Renderable>(entity, s_Textures.SpriteSheetTexture, 25).Sprite;
        auto &preset = Projectile::Presets.at(type);
        projectileSprite.setTextureRect(preset.TextureRect);
        projectileSprite.setOrigin({preset.TextureRect.size.x / 2.0f, preset.TextureRect.size.y / 2.0f});
        projectileSprite.setPosition(position);
        s_Registry.emplace<Velocity>(entity, direction * preset.Speed);
        s_Registry.emplace<Collidable>(entity, preset.CollisionRect, source);
        auto &lifetime = s_Registry.emplace<Lifetime>(entity, sf::seconds(preset.Lifetime));
        lifetime.Clock.restart();
        return entity;
    }

    sf::Vector2u GetWindowSize() {
        return Context.Window.getSize();
    }

    sf::Vector2f GetInputVector() {
        sf::Vector2f inputVector(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            inputVector.x -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            inputVector.x += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            inputVector.y -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            inputVector.y += 1.0f;
        }
        if (inputVector != sf::Vector2f(0.0f, 0.0f)) {
            inputVector = inputVector / std::sqrt(inputVector.x * inputVector.x + inputVector.y * inputVector.y);
        }
        return inputVector;
    }
}

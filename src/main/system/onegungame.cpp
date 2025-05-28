#include "pch.hpp"
#include "onegungame.hpp"

#include "components/components.hpp"

namespace OneGunGame {

    static Data s_Data(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

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
        s_Data.Context.Window = {sf::VideoMode(DefaultWindowSize), WindowTitle, DefaultWindowStyle, DefaultWindowState, DefaultContextSettings};
        s_Data.Context.Window.setFramerateLimit(DefaultFrameRateLimit);

        if (!s_Data.Textures.BackgroundTexture.loadFromFile("./assets/textures/bg1.png")) {
            return std::unexpected<std::string>("Failed to load background texture");
        }
        s_Data.Textures.BackgroundTexture.setSmooth(true);
        if (!s_Data.Textures.SpriteSheetTexture.loadFromFile("./assets/textures/sprites.png")) {
            return std::unexpected<std::string>("Failed to load sprite sheet texture");
        }
        s_Data.Textures.SpriteSheetTexture.setSmooth(true);

        s_Data.EventHandlers.OnClose = [](const sf::Event::Closed&) {
                s_Data.Context.Window.close();
        };
        s_Data.EventHandlers.KeyPressed = [](const sf::Event::KeyPressed& keyEvent) {
            if (keyEvent.code == sf::Keyboard::Key::Escape) {
                s_Data.Context.Window.close();
            }
        };
        
        s_Data.Registry.clear();

        s_Data.Entities.Background = CreateBackground();
        s_Data.Entities.Player = Player::Create(s_Data.Registry, s_Data.Textures.SpriteSheetTexture, Player::Start);

        return 0;
    }

    std::expected<int, std::string> Run() {
        while (s_Data.Context.Window.isOpen()) {
            Update();
            s_Data.Registry.sort<Renderable>([](const Renderable& a, const Renderable& b) {
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
        s_Data.Context.Window.handleEvents(s_Data.EventHandlers.OnClose, s_Data.EventHandlers.KeyPressed);

        sf::Vector2f inputVector = GetInputVector();
        s_Data.Registry.get<Velocity>(s_Data.Entities.Player) = inputVector * Player::DefaultMoveSpeed;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            // DASH
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
            entt::entity newProjectile = Player::Fire(s_Data.Registry, s_Data.Entities.Player, s_Data.Textures.SpriteSheetTexture);
            if (newProjectile != entt::null) {
                s_Data.Entities.Projectiles.push_back(newProjectile);
            }
        }
        
        s_Data.Registry.view<Renderable, Velocity>().each([](auto entity, Renderable& renderable, Velocity& velocity) {
            spdlog::trace("Updating entity {}: Position ({}, {}), Velocity ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y, velocity.x, velocity.y);
            renderable.Sprite.move(velocity);
        });

        s_Data.Registry.view<Lifetime>().each([](auto entity, Lifetime& lifetime) {
            if (lifetime.Clock.getElapsedTime() >= lifetime.Duration) {
                spdlog::trace("Removing entity {} due to lifetime expiration", static_cast<int>(entity));
                if (std::find(s_Data.Entities.Projectiles.begin(), s_Data.Entities.Projectiles.end(), entity) != s_Data.Entities.Projectiles.end()) {
                    s_Data.Entities.Projectiles.erase(std::remove(s_Data.Entities.Projectiles.begin(), s_Data.Entities.Projectiles.end(), entity));
                }
                s_Data.Registry.destroy(entity);
            }
        });
    }

    void Render() {
        s_Data.Context.Window.clear(sf::Color::Black);

        s_Data.Registry.view<Renderable>().each([](auto entity, Renderable& renderable) {
            spdlog::trace("Rendering entity {}: Position ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y);
            s_Data.Context.Window.draw(renderable.Sprite);
        });
        
        s_Data.Context.Window.display();
    }

    entt::entity CreateBackground() {
        entt::entity entity = s_Data.Registry.create();
        s_Data.Registry.emplace<Renderable>(entity, s_Data.Textures.BackgroundTexture, 100);
        return entity;
    }

    sf::Vector2u GetWindowSize() {
        return s_Data.Context.Window.getSize();
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

    entt::registry& GetRegistry() { return s_Data.Registry; }
    RandomGenerator& GetRandomGenerator() { return s_Data.Random; }
    const sf::RenderWindow& GetWindow() { return s_Data.Context.Window; }
}

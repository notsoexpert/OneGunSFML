#include "pch.hpp"
#include "onegungame.hpp"

#include "systems/components.hpp"
#include "entities/entity.hpp"
#include "entities/background.hpp"
#include "entities/player.hpp"
#include "entities/projectile_types.hpp"
#include "entities/enemy_types.hpp"
#include "entities/weapon.hpp"

namespace OneGunGame { 

    static Data s_Data(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

    Data& GetData() {
        return s_Data;
    }

    int Start(int argc, char *argv[]) {
        spdlog::info("spdlog Version: {0}.{1}.{2}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
        spdlog::info("SFML Version: {0}.{1}.{2}", SFML_VERSION_MAJOR, SFML_VERSION_MINOR, SFML_VERSION_PATCH);
        spdlog::info("EnTT Version: {0}.{1}.{2}", ENTT_VERSION_MAJOR, ENTT_VERSION_MINOR, ENTT_VERSION_PATCH);

        spdlog::info("Argc: {0}", argc);
        for (auto i : std::ranges::iota_view{0, argc}) {
            spdlog::info("Argv[{0}]: {1}", i, argv[i]);

            std::string_view arg{argv[i]};

            if (arg.starts_with("-l")) {
                if (arg.ends_with("0")) {
                    spdlog::set_level(spdlog::level::trace);
                } else if (arg.ends_with("1")) {
                    spdlog::set_level(spdlog::level::info);
                } else if (arg.ends_with("2")) {
                    spdlog::set_level(spdlog::level::warn);
                } else if (arg.ends_with("3")) {
                    spdlog::set_level(spdlog::level::err);
                } else if (arg.ends_with("4")) {
                    spdlog::set_level(spdlog::level::critical);
                }
            }

            if (arg.starts_with("-f")) {
                Configuration.WindowState = sf::State::Fullscreen;
            }
        }

        spdlog::info("Setting up OneGunGame...");
        auto setupResult = Setup(/*pass command line arguments for specific window config*/);
        if (!setupResult) {
            spdlog::error("Setup failed: {}", setupResult.error());
            return 1;
        }
        spdlog::info("Setup completed successfully.");

        spdlog::info("Running OneGunGame...");
        Run();
        spdlog::info("Run completed successfully.");

        spdlog::info("Cleaning up OneGunGame...");
        Cleanup();
        spdlog::info("Cleanup completed successfully.");

        return 0;
    }

    std::expected<void, std::string> Setup() {
        SetupContext();
        if (auto setupResult = SetupTextures(); !setupResult) {
            return setupResult;
        }
        SetupEventHandlers();
        SetupRegistry();
        SetupLevel();

        return {};
    }

    void Run() {
        while (s_Data.Context.Window.isOpen()) {
            Update();
            s_Data.Registry.sort<Renderable>([](const Renderable& a, const Renderable& b) {
                return a.DrawOrder > b.DrawOrder;
            });
            Render();
        }
    }

    void Cleanup() {
    }

    void Update() {
        s_Data.Context.Window.handleEvents(s_Data.EventHandlers.OnClose, s_Data.EventHandlers.KeyPressed);

        // Handle Inputs
        Player::Update(s_Data.Registry, s_Data.Entities.Player);

        // Handle Enemy Behavior
        Enemy::Update(s_Data.Registry);
        
        // Handle Projectile Behavior
        Projectile::Update(s_Data.Registry);

        ProcessMovement();

        Background::Update(s_Data.Registry);

        ApplyMovement();

        UpdateAnimations();

        CheckCollisions();

        Entity::Update(s_Data.Registry);

        ProcessDeaths();

        RecycleEntities();
    }

    void Render() {
        s_Data.Context.Window.clear(sf::Color::Black);

        s_Data.Registry.view<Renderable>().each(
            [](auto entity, Renderable& renderable) {
            spdlog::trace("Rendering entity {}: Position ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y);
            s_Data.Context.Window.draw(renderable.Sprite);
#if DEBUGN
            s_Data.Context.Window.draw(renderable.DebugRect);
#endif
        });
        
        s_Data.Context.Window.display();
    }

    sf::Vector2f GetWindowSize() {
        return ViewSize;
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

    uint32_t GetEntityCount() {
        uint32_t total = 0;
        for ([[maybe_unused]]auto entity : s_Data.Registry.view<entt::entity>()) {
            total += 1;
        }
        return total;
    }

    const sf::Texture& GetTexture(Images image) {
        return s_Data.Textures[image].Texture;
    }
    entt::registry& GetRegistry() { return s_Data.Registry; }
    entt::entity GetPlayerEntity() { return s_Data.Entities.Player; }
    RandomGenerator& GetRandomGenerator() { return s_Data.Random; }
    const sf::RenderWindow& GetWindow() { return s_Data.Context.Window; }
}

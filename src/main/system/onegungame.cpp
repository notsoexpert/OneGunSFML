#include "pch.hpp"
#include "onegungame.hpp"

#include "actors/player.hpp"

namespace OneGunGame {

    struct {
        sf::RenderWindow Window;
    } static Context;

    struct {
        sf::Texture BackgroundTexture;
        sf::Texture SpriteSheetTexture;
    } static Textures;

    struct {
        std::function<void(const sf::Event::Closed&)> OnClose;
        std::function<void(const sf::Event::KeyPressed&)> KeyPressed;
    } static EventHandlers;

    struct {
        entt::entity Background;
        entt::entity Player;
        std::vector<entt::entity> Projectiles;
        std::vector<entt::entity> Enemies;
    } static Entities;

    struct Renderable {
        sf::Sprite Sprite;
        int DrawOrder;
        Renderable(const sf::Texture& texture, int drawOrder = 0) : Sprite(texture), DrawOrder(drawOrder) {}
    };

    using Velocity = sf::Vector2f;

    static entt::registry Registry;

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

        if (!Textures.BackgroundTexture.loadFromFile("../assets/textures/bg1.png")) {
            spdlog::info("Failed to load background texture");
            return -1;
        }
        Textures.BackgroundTexture.setSmooth(true);
        if (!Textures.SpriteSheetTexture.loadFromFile("../assets/textures/sprites.png")) {
            spdlog::info("Failed to load sprite sheet texture");
            return -1;
        }
        Textures.SpriteSheetTexture.setSmooth(true);

        EventHandlers.OnClose = [](const sf::Event::Closed&) {
                Context.Window.close();
        };
        EventHandlers.KeyPressed = [](const sf::Event::KeyPressed& keyEvent) {
            if (keyEvent.code == sf::Keyboard::Key::Escape) {
                Context.Window.close();
            }
        };
        
        Registry.clear();

        Entities.Background = Registry.create();
        Registry.emplace<Renderable>(Entities.Background, Textures.BackgroundTexture, 100);

        Entities.Player = Registry.create();
        auto& playerSprite = Registry.emplace<Renderable>(Entities.Player, Textures.SpriteSheetTexture, 10).Sprite;
        playerSprite.setTextureRect({{0, 0}, Player::Size});
        playerSprite.setOrigin({Player::Size.x / 2.0f, Player::Size.y / 2.0f});
        playerSprite.setPosition(Player::Start);
        Registry.emplace<Velocity>(Entities.Player, 0.0f, 0.0f);

        return 0;
    }

    std::expected<int, std::string> Run() {
        while (Context.Window.isOpen()) {
            Update();
            Registry.sort<Renderable>([](const Renderable& a, const Renderable& b) {
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
        Context.Window.handleEvents(EventHandlers.OnClose, EventHandlers.KeyPressed);

        sf::Vector2f inputVector = GetInputVector();
        Registry.get<Velocity>(Entities.Player) = inputVector * Player::DefaultMoveSpeed;
        
        Registry.view<Renderable, Velocity>().each([](auto entity, Renderable& renderable, Velocity& velocity) {
            spdlog::trace("Updating entity {}: Position ({}, {}), Velocity ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y, velocity.x, velocity.y);
            renderable.Sprite.move(velocity);
        });
    }

    void Render() {
        Context.Window.clear(sf::Color::Black);

        Registry.view<Renderable>().each([](auto entity, Renderable& renderable) {
            spdlog::trace("Rendering entity {}: Position ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y);
            Context.Window.draw(renderable.Sprite);
        });
        
        Context.Window.display();
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

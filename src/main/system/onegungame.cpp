#include "pch.hpp"
#include "onegungame.hpp"

#include "components/components.hpp"
#include "actors/background.hpp"
#include "actors/player.hpp"
#include "actors/projectile.hpp"
#include "actors/enemy.hpp"

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

        if (!s_Data.Textures.BackgroundTexture.loadFromFile(OneGunGame::BackgroundPath)) {
            return std::unexpected<std::string>("Failed to load background texture");
        }
        s_Data.Textures.BackgroundTexture.setSmooth(true);
        s_Data.Textures.BackgroundTexture.setRepeated(true);
        if (!s_Data.Textures.SpriteSheetTexture.loadFromFile(OneGunGame::SpriteSheetPath)) {
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

        s_Data.Entities.Background = Background::Create(s_Data.Registry, s_Data.Textures.BackgroundTexture);
        s_Data.Entities.Player = Player::Create(s_Data.Registry, s_Data.Textures.SpriteSheetTexture, Player::Start(s_Data.Context.Window.getSize()));

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

    bool FilterCollidable(OneGunGame::CollisionLayer mask, OneGunGame::CollisionLayer type) {
        return (mask & type) != 0;
    }

    void Update() {
        s_Data.Context.Window.handleEvents(s_Data.EventHandlers.OnClose, s_Data.EventHandlers.KeyPressed);

        sf::Vector2f inputVector = GetInputVector();
        Player::Move(inputVector, s_Data.Registry, s_Data.Entities.Player);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            Player::Dash(s_Data.Registry, s_Data.Entities.Player);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
            auto newProjectile = Player::Fire(s_Data.Registry, s_Data.Entities.Player, s_Data.Textures.SpriteSheetTexture);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) {
            sf::Vector2f spawnPosition = {s_Data.Random.generateFloat(-50.0f, GetWindowSize().x + 50.0f),
                                            s_Data.Random.generateFloat(-256.0f, -64.0f)};
            sf::Vector2f flyDirection = {0.0f, 1.0f};
            flyDirection = flyDirection.rotatedBy(sf::radians(s_Data.Random.generateFloat(-HalfPi, HalfPi)));
            
            Enemy::Create(s_Data.Registry, static_cast<Enemy::Type>(s_Data.Random.generateInt(0, 2)),
                            s_Data.Textures.SpriteSheetTexture, spawnPosition, flyDirection);
        }

        s_Data.Registry.view<Acceleration, Velocity>().each(
            [](auto entity, Acceleration& acceleration, Velocity& velocity) {
            spdlog::trace("Updating entity {}: Velocity ({}, {}) with Acceleration ({}, {})",
                static_cast<int>(entity), velocity.Value.x, velocity.Value.y, acceleration.Value.x, acceleration.Value.y);
            velocity.Value.x += acceleration.Value.x;
            velocity.Value.y += acceleration.Value.y;
            if (std::abs(velocity.Value.x) < 0.01f) velocity.Value.x = 0.0f;
            if (std::abs(velocity.Value.y) < 0.01f) velocity.Value.y = 0.0f;
        });

        s_Data.Registry.view<MaxSpeed, Velocity>().each([](auto entity, MaxSpeed& maxSpeed, Velocity& velocity) {
            spdlog::trace("Clamping speed for entity {}: Velocity ({}, {}) with MaxSpeed ({})", 
                static_cast<int>(entity), velocity.Value.x, velocity.Value.y, maxSpeed.Value);

            if (velocity.Value.length() <= maxSpeed.Value) return;
            velocity.Value = velocity.Value.normalized() * maxSpeed.Value;
        });

        s_Data.Registry.view<Friction, Velocity>().each(
            [](auto Entity, Friction& friction, Velocity& velocity) {
            spdlog::trace("Applying friction to entity {}: Velocity ({}, {}) with Friction ({})",
                static_cast<int>(Entity), velocity.Value.x, velocity.Value.y, friction.Value);
            velocity.Value.x *= (1.0f - friction.Value);
            velocity.Value.y *= (1.0f - friction.Value);
            if (std::abs(velocity.Value.x) < 0.01f) velocity.Value.x = 0.0f;
            if (std::abs(velocity.Value.y) < 0.01f) velocity.Value.y = 0.0f;
        });

        s_Data.Registry.view<Renderable, Velocity, Background::VerticalLoop>().each(
            [&](auto entity, Renderable &renderable, Velocity &velocity, Background::VerticalLoop &vLoop) {
            spdlog::trace("Updating entity {}: vLoop LimitY: {}, ResetY: {}, Copy Entity: {}", 
                static_cast<int>(entity), vLoop.LimitY, vLoop.ResetY, static_cast<int>(vLoop.BackgroundCopy));
            auto pos = renderable.Sprite.getPosition();
            const auto &vel = velocity.Value;
            
            //spdlog::warn("BG Pos: ({}, {}), BG Vel: ({}, {}) LimitY: {}", pos.x, pos.y, vel.x, vel.y, vLoop.LimitY);
            if (pos.y + vel.y <= vLoop.LimitY) 
                return;
            
            auto &copyRenderable = s_Data.Registry.get<Renderable>(vLoop.BackgroundCopy);
            renderable.Sprite.setPosition({pos.x, vLoop.ResetY});
            copyRenderable.Sprite.setPosition({pos.x, vLoop.ResetY - copyRenderable.Sprite.getTexture().getSize().y});
        });
        
        s_Data.Registry.view<Renderable, Velocity>().each(
            [](auto entity, Renderable& renderable, Velocity& velocity) {
            spdlog::trace("Updating entity {}: Position ({}, {}), Velocity ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y, velocity.Value.x, velocity.Value.y);
            renderable.Sprite.move(velocity.Value);
        });

        s_Data.Registry.view<Renderable, Background::HorizontalParallax>().each(
            [&](auto entity, Renderable& renderable, Background::HorizontalParallax& parallax) {
            spdlog::trace("Updating entity {}: Parallax value ({})", static_cast<int>(entity), parallax.Value);
            const auto &playerPosition = s_Data.Registry.get<Renderable>(s_Data.Entities.Player).Sprite.getPosition();
            parallax.Value = playerPosition.x / OneGunGame::GetWindowSize().x;
            renderable.Sprite.setPosition({OneGunGame::GetWindowSize().x * -parallax.Value, renderable.Sprite.getPosition().y});
        });

        s_Data.Registry.view<Renderable, Confined>().each(
            [&](auto entity, Renderable &renderable, Confined &confined) {
            spdlog::trace("Updating entity {}: Confined to ({}, {}, {}, {})", static_cast<int>(entity), 
                confined.Limits.position.x, confined.Limits.position.y, confined.Limits.size.x, confined.Limits.size.y);
            auto position = renderable.Sprite.getPosition();
            position.x = std::clamp(position.x, confined.Limits.position.x, confined.Limits.position.x + confined.Limits.size.x);
            position.y = std::clamp(position.y, confined.Limits.position.y, confined.Limits.position.y + confined.Limits.size.y);
            renderable.Sprite.setPosition(position);
        });

        s_Data.Registry.view<Collidable>().each(
            [](auto entity, Collidable& collidable) {
            spdlog::trace("Checking collisions for entity {}: CollisionRect ({}, {}, {}, {})", 
                static_cast<int>(entity), collidable.CollisionRect.position.x, collidable.CollisionRect.position.y, collidable.CollisionRect.size.x, collidable.CollisionRect.size.y);
            auto &rectA = collidable.CollisionRect;
            s_Data.Registry.view<Collidable>().each([&](auto otherEntity, Collidable& otherCollidable) {
                if (entity == otherEntity || otherEntity == collidable.Source) 
                    return;
                
                if (!FilterCollidable(collidable.Mask, otherCollidable.Layer)) {
                    return;
                };
                
                auto &rectB = otherCollidable.CollisionRect;
                if (rectA.findIntersection(rectB)) {
                    spdlog::trace("Collision detected between entity {} and entity {}", 
                        static_cast<int>(entity), static_cast<int>(otherEntity));
                    // auto collisionEntity = s_Data.Registry.create();
                    
                    // s_Data.Registry.emplace<Collision>(collisionEntity, entity, otherEntity);


                }
            });
        });

        // s_Data.Registry.view<Collision>().each([](auto entity, Collision& collision) {
        //     spdlog::trace("Processing {} collision between entity {} and entity {}", static_cast<int>(entity),
        //         static_cast<int>(collision.EntityA), static_cast<int>(collision.EntityB));
            
        //         s_Data.Registry.destroy(entity);
        // });

        s_Data.Registry.view<Lifetime>().each(
            [](auto entity, Lifetime& lifetime) {
            if (lifetime.Clock.getElapsedTime() >= lifetime.Duration) {
                spdlog::trace("Removing entity {} due to lifetime expiration", static_cast<int>(entity));
                s_Data.Registry.destroy(entity);
            }
        });
    }

    void Render() {
        s_Data.Context.Window.clear(sf::Color::Black);

        s_Data.Registry.view<Renderable>().each(
            [](auto entity, Renderable& renderable) {
            spdlog::trace("Rendering entity {}: Position ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y);
            s_Data.Context.Window.draw(renderable.Sprite);
        });
        
        s_Data.Context.Window.display();
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

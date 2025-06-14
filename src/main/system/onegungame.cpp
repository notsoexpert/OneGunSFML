#include "pch.hpp"
#include "onegungame.hpp"

#include "system/components.hpp"
#include "entities/entity.hpp"
#include "entities/background.hpp"
#include "entities/player.hpp"
#include "entities/projectile_types.hpp"
#include "entities/enemy_types.hpp"

namespace OneGunGame { 

    static Data s_Data(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

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
            return -1;
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
        auto setupResult = SetupTextures();
        if (!setupResult) {
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

    bool FilterCollidable(OneGunGame::CollisionLayer mask, OneGunGame::CollisionLayer type) {
        return (mask & type) != 0;
    }

    void Update() {
        s_Data.Context.Window.handleEvents(s_Data.EventHandlers.OnClose, s_Data.EventHandlers.KeyPressed);

        // Handle Inputs
        Player::Update(s_Data.Registry, s_Data.Entities.Player);

        // Handle Enemy Behavior
        Enemy::Update(s_Data.Registry);
        

        // TODO: Use DeltaTime
        s_Data.Registry.view<Acceleration, Velocity>().each(
            [](auto entity, Acceleration& acceleration, Velocity& velocity) {
            spdlog::trace("Updating entity {}: Velocity ({}, {}) with Acceleration ({}, {})",
                static_cast<int>(entity), velocity.Value.x, velocity.Value.y, acceleration.Value.x, acceleration.Value.y);
            velocity.Value.x += acceleration.Value.x;
            velocity.Value.y += acceleration.Value.y;
            if (std::abs(velocity.Value.x) < 0.01f) velocity.Value.x = 0.0f;
            if (std::abs(velocity.Value.y) < 0.01f) velocity.Value.y = 0.0f;
        });

        // TODO: Use DeltaTime
        s_Data.Registry.view<Friction, Velocity>().each(
            [](auto Entity, Friction& friction, Velocity& velocity) {
            spdlog::trace("Applying friction to entity {}: Velocity ({}, {}) with Friction ({})",
                static_cast<int>(Entity), velocity.Value.x, velocity.Value.y, friction.Value);
            velocity.Value.x *= (1.0f - friction.Value);
            velocity.Value.y *= (1.0f - friction.Value);
            if (std::abs(velocity.Value.x) < 0.01f) velocity.Value.x = 0.0f;
            if (std::abs(velocity.Value.y) < 0.01f) velocity.Value.y = 0.0f;
        });

        s_Data.Registry.view<MaxSpeed, Velocity>().each([](auto entity, MaxSpeed& maxSpeed, Velocity& velocity) {
            spdlog::trace("Clamping speed for entity {}: Velocity ({}, {}) with MaxSpeed ({})", 
                static_cast<int>(entity), velocity.Value.x, velocity.Value.y, maxSpeed.Value);

            if (velocity.Value.length() <= maxSpeed.Value) return;
            velocity.Value = velocity.Value.normalized() * maxSpeed.Value;
        });

        s_Data.Registry.view<Renderable, Animation>().each(
            [](Renderable& renderable, Animation& animation) {
                if (!animation.IsFrameComplete()) {
                    return;
                }

                auto texRect = renderable.Sprite.getTextureRect();
                if (animation.CurrentFrame + 1 >= animation.TotalFrames) {
                    texRect.position = animation.TextureRectStartPosition;
                    animation.CurrentFrame = 0;
                } else {
                    texRect.position.x += texRect.size.x;
                    animation.CurrentFrame++;
                }
                renderable.Sprite.setTextureRect(texRect);
            }
        );

        s_Data.Registry.view<Renderable, Scaling>().each(
            [](auto entity, Renderable& renderable, Scaling& scaling) {
                if (scaling.IsScaleComplete()) {
                    renderable.Sprite.setScale(scaling.TargetScale);
                    s_Data.Registry.remove<Scaling>(entity);
                    return;
                }
                float scalePercentage = scaling.GetScalePercentage();
                spdlog::trace("Scale percent: {}", scalePercentage);
                sf::Vector2f scaleFactor = scaling.OriginalScale * (1 - scalePercentage) + scaling.TargetScale * scalePercentage;
                renderable.Sprite.setScale(scaleFactor);
            }
        );

        s_Data.Registry.view<Renderable, Fading>().each(
            [](Renderable& renderable, Fading& fading) {
                auto color = renderable.Sprite.getColor();
                if (fading.IsFadeComplete()) {
                    color.a = fading.TargetAlpha;
                    renderable.Sprite.setColor(color);
                    return;
                }
                float fadePercentage = fading.GetFadePercentage();
                spdlog::trace("Fade percent: {}", fadePercentage);
                color.a = static_cast<uint8_t>(fading.OriginalAlpha * (1 - fadePercentage) + fading.TargetAlpha * fadePercentage);
                renderable.Sprite.setColor(color);
            }
        );

        Background::Update(s_Data.Registry);

        // TODO: Use DeltaTime
        s_Data.Registry.view<Renderable, Rotating>().each(
            [](auto entity, Renderable &renderable, Rotating &rotating) {
                spdlog::trace("Updating entity {}: Rotating {} radians per second", 
                static_cast<int>(entity), rotating.RotationsPerSecond.asRadians());
                renderable.Sprite.rotate(rotating.RotationsPerSecond / static_cast<float>(Configuration.FrameRateLimit));
            }
        );
        
        // TODO: Use DeltaTime
        s_Data.Registry.view<Renderable, Velocity>().each(
            [](auto entity, Renderable& renderable, Velocity& velocity) {
            spdlog::trace("Updating entity {}: Position ({}, {}), Velocity ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y, velocity.Value.x, velocity.Value.y);
            renderable.Sprite.move(velocity.Value);
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

        CheckCollisions();

        Entity::Update(s_Data.Registry);

        s_Data.Registry.view<Attachment>().each(
            [](auto entity, Attachment& attachment) {
                if (!s_Data.Registry.valid(attachment.Parent)) {
                    s_Data.Registry.emplace_or_replace<Dying>(entity, attachment.OnDetach);
                    s_Data.Registry.remove<Attachment>(entity);
                    return;
                }
                if (s_Data.Registry.any_of<Destructing, Dying>(attachment.Parent)) {
                    s_Data.Registry.emplace_or_replace<Dying>(entity, attachment.OnDetach);
                    s_Data.Registry.remove<Attachment>(entity);
                    return;
                }
            }
        );

        s_Data.Registry.view<Dying>().each(
            [](auto entity, Dying& dying) {
                if (dying.OnDeath) {
                    dying.OnDeath(s_Data.Registry, entity);
                }
                s_Data.Registry.remove<Dying>(entity);
            }
        );

        s_Data.Registry.view<Lifetime>().each(
            [](auto entity, Lifetime& lifetime) {
            if (lifetime.Clock.getElapsedTime() >= lifetime.Duration) {
                spdlog::trace("Destructing entity {} due to lifetime expiration", static_cast<int>(entity));
                s_Data.Registry.emplace_or_replace<Destructing>(entity);
            }
        });

        s_Data.Registry.view<Destructing>().each(
            [](auto entity) {
                s_Data.Registry.destroy(entity);
            }
        );
    }

    void Render() {
        s_Data.Context.Window.clear(sf::Color::Black);

        s_Data.Registry.view<Renderable>().each(
            [](auto entity, Renderable& renderable) {
            spdlog::trace("Rendering entity {}: Position ({}, {})", 
                static_cast<int>(entity), renderable.Sprite.getPosition().x, renderable.Sprite.getPosition().y);
            s_Data.Context.Window.draw(renderable.Sprite);
#if DEBUG
            s_Data.Context.Window.draw(renderable.DebugRect);
#endif
        });
        
        s_Data.Context.Window.display();
    }

    sf::Vector2f GetAdjustedCollisionRectSize(const sf::IntRect& originalRect, const sf::Vector2f& renderableScale) {
        return {originalRect.size.x * renderableScale.x, originalRect.size.y * renderableScale.y};
    }

    sf::IntRect GetAdjustedCollisionRect(const sf::IntRect& originalRect, const sf::Vector2f& renderablePosition, const sf::Vector2f& renderableScale) {
        auto newSize = GetAdjustedCollisionRectSize(originalRect, renderableScale);
        return sf::IntRect{RoundVector(static_cast<sf::Vector2f>(originalRect.position) + renderablePosition - (newSize / 2.0f)), RoundVector(newSize)};
    }

    void CheckCollisions() {
        std::unordered_map<entt::entity, std::unordered_map<entt::entity, bool>> processedCollisions;
        s_Data.Registry.view<Renderable, Collidable>().each([&](auto entity, Renderable &renderable, Collidable& collidable) {
            spdlog::trace("Checking collisions for entity {}: CollisionRect ({}, {}, {}, {})", 
                static_cast<int>(entity), collidable.CollisionRect.position.x, collidable.CollisionRect.position.y, collidable.CollisionRect.size.x, collidable.CollisionRect.size.y);
            auto rectA = GetAdjustedCollisionRect(collidable.CollisionRect, 
                renderable.Sprite.getPosition(), renderable.Sprite.getScale());

#if DEBUG
            renderable.DebugRect.setSize(static_cast<sf::Vector2f>(rectA.size));
            renderable.DebugRect.setOutlineColor(sf::Color::Red);
            renderable.DebugRect.setOutlineThickness(1.0f);
            renderable.DebugRect.setFillColor(sf::Color::Transparent);
            renderable.DebugRect.setPosition(static_cast<sf::Vector2f>(rectA.position));
#endif

            processedCollisions[entity] = {};
            for (auto [otherEntity, otherRenderable, otherCollidable] : s_Data.Registry.view<Renderable, Collidable>().each()) {
                if (s_Data.Registry.any_of<Destructing, Dying>(entity)) {
                   return;
                }
                processedCollisions[entity][otherEntity] = true;
                if (s_Data.Registry.any_of<Destructing, Dying>(otherEntity)) {
                   continue;
                }
                if (processedCollisions.contains(otherEntity) && processedCollisions[otherEntity].contains(entity)) {
                    continue;
                }
                if (entity == otherEntity || otherEntity == collidable.Source) {
                    continue;
                }
                if (!FilterCollidable(collidable.Mask, otherCollidable.Layer)) {
                    continue;
                }

                auto rectB = GetAdjustedCollisionRect(otherCollidable.CollisionRect, 
                    otherRenderable.Sprite.getPosition(), otherRenderable.Sprite.getScale());
                if (rectA.findIntersection(rectB)) {
                    spdlog::trace("Collision detected between entity {} and entity {}", 
                        static_cast<int>(entity), static_cast<int>(otherEntity));
                    auto forwardCollision = Collision{s_Data.Registry, entity, otherEntity};
                    auto reverseCollision = Collision{s_Data.Registry, otherEntity, entity};
                    collidable.OnCollision(forwardCollision);
                    otherCollidable.OnCollision(reverseCollision);
                }
            }
        });
    }

    void SetupContext() {
        s_Data.Context.Window = sf::RenderWindow(sf::VideoMode(Configuration.WindowSize), WindowTitle, WindowStyle, Configuration.WindowState, Configuration.ContextSettings);
        s_Data.Context.Window.setFramerateLimit(Configuration.FrameRateLimit);
        s_Data.Context.View.setCenter(sf::Vector2f{ViewSize.x / 2, ViewSize.y / 2});
        s_Data.Context.View.setSize(static_cast<sf::Vector2f>(ViewSize));

        s_Data.Context.Window.setView(s_Data.Context.View);
    }

    std::expected<void, std::string> SetupTextures() {
        if (!LoadTexture(Background, true, true)) {
            return std::unexpected<std::string>("Failed to load background texture");
        }
        if (!LoadTexture(SpriteSheet, true)) {
            return std::unexpected<std::string>("Failed to load sprite sheet texture");
        }
        if (!LoadTexture(ExplosionRed, true)) {
            return std::unexpected<std::string>("Failed to load red explosion texture");
        }
        if (!LoadTexture(ExplosionYellow, true)) {
            return std::unexpected<std::string>("Failed to load yellow explosion texture");
        }
        if (!LoadTexture(ExplosionGreen, true)) {
            return std::unexpected<std::string>("Failed to load green explosion texture");
        }
        if (!LoadTexture(ExplosionBlue, true)) {
            return std::unexpected<std::string>("Failed to load blue explosion texture");
        }
        if (!LoadTexture(ExplosionViolet, true)) {
            return std::unexpected<std::string>("Failed to load violet explosion texture");
        }
        if (!GeneratePlaceholderTexture()) {
            return std::unexpected<std::string>("Failed to generate placeholder texture");
        }
        return {};
    }
    void SetupEventHandlers() {
        s_Data.EventHandlers.OnClose = [](const sf::Event::Closed&) {
                s_Data.Context.Window.close();
        };
        s_Data.EventHandlers.KeyPressed = [](const sf::Event::KeyPressed& keyEvent) {
            if (keyEvent.code == sf::Keyboard::Key::Escape) {
                s_Data.Context.Window.close();
            }
            if (keyEvent.code == sf::Keyboard::Key::T) {
                spdlog::error("TOTAL ENTITIES: {}", GetEntityCount());
            }
            if (keyEvent.code == sf::Keyboard::Key::N) {
                sf::Vector2f spawnPosition = {s_Data.Random.generateFloat(-50.0f, GetWindowSize().x + 50.0f),
                                            s_Data.Random.generateFloat(-96.0f, -32.0f)};
                sf::Vector2f flyDirection = {0.0f, 1.0f};
                flyDirection = flyDirection.rotatedBy(sf::radians(s_Data.Random.generateFloat(-HalfPi, HalfPi)));
                
                Enemy::Setup setup{s_Data.Registry, spawnPosition, flyDirection, entt::null, entt::null};
                Enemy::Create(setup, static_cast<Enemy::Type>(s_Data.Random.generateInt(static_cast<int>(Enemy::Type::Asteroid), static_cast<int>(Enemy::Type::Drone))));
            }
            if (keyEvent.code == sf::Keyboard::Key::Q) {
                auto &weapon = s_Data.Registry.get<Projectile::Weapon>(s_Data.Entities.Player);
                weapon.ProjectileType = static_cast<Projectile::Type>((static_cast<size_t>(weapon.ProjectileType) + 1) % static_cast<size_t>(Projectile::Type::Total));
            }
        };
    }
    void SetupRegistry() {
        s_Data.Registry.clear();
    }
    void SetupLevel() {
        s_Data.Entities.Background = Background::Create(s_Data.Registry);
        s_Data.Entities.Player = Player::Create(s_Data.Registry, Player::Start(GetWindowSize()));
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

    CollisionLayer GetHitMask(CollisionLayer layer){
        switch (layer) {
            case CollisionLayer::Player:
                return static_cast<CollisionLayer>
                (CollisionLayer::Enemy | CollisionLayer::Obstacle);
            case CollisionLayer::Projectile:
                return static_cast<CollisionLayer>
                (CollisionLayer::Player | CollisionLayer::Enemy | 
                    CollisionLayer::Obstacle);
            case CollisionLayer::Enemy:
                return static_cast<CollisionLayer>
                (CollisionLayer::Player | CollisionLayer::Obstacle);
            case CollisionLayer::Obstacle:
                return static_cast<CollisionLayer>
                (CollisionLayer::Player | CollisionLayer::Projectile | 
                    CollisionLayer::Enemy);
        }
        return CollisionLayer::Obstacle;
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

    sf::Vector2i RoundVector(sf::Vector2f vec) {
        return sf::Vector2i{RoundCoordinate(vec.x), RoundCoordinate(vec.y)};
    }

    int RoundCoordinate(float coord) {
        return static_cast<int>(std::round(coord));
    }

    entt::registry& GetRegistry() { return s_Data.Registry; }
    entt::entity GetPlayerEntity() { return s_Data.Entities.Player; }
    RandomGenerator& GetRandomGenerator() { return s_Data.Random; }
    const sf::RenderWindow& GetWindow() { return s_Data.Context.Window; }

    bool LoadTexture(Images imgType, bool smooth, bool repeat) {
        if (s_Data.Textures[imgType].Load()) {
            s_Data.Textures[imgType].Texture.setSmooth(smooth);
            s_Data.Textures[imgType].Texture.setRepeated(repeat);
            return true;
        }
        return false;
    }

    bool GeneratePlaceholderTexture() {
        sf::Image img{{2u, 2u}, sf::Color::Magenta};
        return s_Data.Textures[Unknown].Texture.loadFromImage(img);
    }
}

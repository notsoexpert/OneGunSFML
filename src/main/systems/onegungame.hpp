#pragma once

#include "systems/constants.hpp"
#include "systems/randomgenerator.hpp"

struct Renderable;
struct Collidable;

namespace OneGunGame {
    int Start(int argc, char *argv[]);
    std::expected<void, std::string> Setup();
    void SetupContext();
    std::expected<void, std::string> SetupTextures();
    void SetupEventHandlers();
    void SetupRegistry();
    void SetupLevel();
    void Run();
    void Cleanup();
    
    void Update();
    void Sort();
    void Render();

    sf::Vector2f GetInputVector();

    sf::Vector2f GetWindowSize();
    uint32_t GetEntityCount();
    const sf::Texture& GetTexture(Images image);

    entt::registry& GetRegistry();
    entt::entity GetPlayerEntity();
    RandomGenerator& GetRandomGenerator();
    const sf::RenderWindow& GetWindow();

    void CheckCollisions();
    void ProcessMovement();
    void UpdateAnimations();
    void ApplyMovement();
    void ProcessDeaths();
    void RecycleEntities();

    struct Data {
        entt::registry Registry;
        RandomGenerator Random;
        
        struct {
            sf::RenderWindow Window;
            sf::View View;
        } Context;

        struct TexData {
            const char* Path;
            sf::Texture Texture;

            bool Load() {
                return Texture.loadFromFile(Path);
            }
        };
        std::unordered_map<Images, TexData> Textures {
            {Images::BG, TexData{"./assets/textures/bg1seamless.png", {}}},
            {Images::SpriteSheet, TexData{"./assets/textures/sprites.png", {}}},
            {Images::ExplosionRed, TexData{"./assets/textures/explosionRed.png", {}}},
            {Images::ExplosionYellow, TexData{"./assets/textures/explosionYellow.png", {}}},
            {Images::ExplosionGreen, TexData{"./assets/textures/explosionGreen.png", {}}},
            {Images::ExplosionBlue, TexData{"./assets/textures/explosionBlue.png", {}}},
            {Images::ExplosionViolet, TexData{"./assets/textures/explosionViolet.png", {}}}
        };

        struct {
            std::function<void(const sf::Event::Closed&)> OnClose;
            std::function<void(const sf::Event::KeyPressed&)> KeyPressed;
        } EventHandlers;
        struct {
            entt::entity Background;
            entt::entity Player;
        } Entities;

        Data(unsigned int seed)
            : Random(seed) {}
    };

    Data& GetData();

    bool LoadTexture(Images imgType, bool smooth = false, bool repeat = false);
    bool GeneratePlaceholderTexture();

}
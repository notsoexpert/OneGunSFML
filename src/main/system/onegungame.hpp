#pragma once

#include "system/constants.hpp"
#include "system/randomgenerator.hpp"

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
    void Render();

    sf::Vector2f GetInputVector();

    sf::Vector2f GetWindowSize();
    uint32_t GetEntityCount();
    const sf::Texture& GetTexture(Images image);

    constexpr float GetDistanceSquared(const sf::Vector2f& a, const sf::Vector2f& b){
        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
    }
    constexpr int RoundCoordinate(float coord) {
        if (coord >= 0.0f) {
            return static_cast<int>(coord + 0.5f);
        }
        return static_cast<int>(coord - 0.5f);
    }
    constexpr sf::Vector2i RoundVector(sf::Vector2f vec){
        return sf::Vector2i{RoundCoordinate(vec.x), RoundCoordinate(vec.y)};
    }

    entt::registry& GetRegistry();
    entt::entity GetPlayerEntity();
    RandomGenerator& GetRandomGenerator();
    const sf::RenderWindow& GetWindow();

    void CheckCollisions();

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
            {Background, TexData{"./assets/textures/bg1seamless.png", {}}},
            {SpriteSheet, TexData{"./assets/textures/sprites.png", {}}},
            {ExplosionRed, TexData{"./assets/textures/explosionRed.png", {}}},
            {ExplosionYellow, TexData{"./assets/textures/explosionYellow.png", {}}},
            {ExplosionGreen, TexData{"./assets/textures/explosionGreen.png", {}}},
            {ExplosionBlue, TexData{"./assets/textures/explosionBlue.png", {}}},
            {ExplosionViolet, TexData{"./assets/textures/explosionViolet.png", {}}}
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

    bool LoadTexture(Images imgType, bool smooth = false, bool repeat = false);
    bool GeneratePlaceholderTexture();

}
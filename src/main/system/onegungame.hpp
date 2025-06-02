#pragma once

#include "system/constants.hpp"
#include "system/randomgenerator.hpp"

namespace OneGunGame {
    int Start(int argc, char *argv[]);
    std::expected<int, std::string> Setup();
    std::expected<int, std::string> Run();
    std::expected<int, std::string> Cleanup();
    
    void Update();
    void Render();

    sf::Vector2f GetInputVector();
    sf::Vector2u GetWindowSize();
    uint32_t GetEntityCount();
    const sf::Texture& GetTexture(Images image);

    sf::Vector2i RoundVector(sf::Vector2f vec);
    int RoundCoordinate(float coord);

    entt::registry& GetRegistry();
    entt::entity GetPlayerEntity();
    RandomGenerator& GetRandomGenerator();
    const sf::RenderWindow& GetWindow();

    struct Data {
        entt::registry Registry;
        RandomGenerator Random;
        
        struct {
            sf::RenderWindow Window;
        } Context;
        struct {
            sf::Texture BackgroundTexture;
            sf::Texture SpriteSheetTexture;
            sf::Texture UnknownTexture;
        } Textures;
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

}
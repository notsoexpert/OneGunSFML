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

    entt::registry& GetRegistry();
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
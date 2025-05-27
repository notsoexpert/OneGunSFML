#pragma once

namespace OneGunGame {
    constexpr sf::Vector2u DefaultWindowSize{800, 600};
    constexpr sf::ContextSettings DefaultContextSettings{24, 8, 4, 3, 3}; // 24 bits color depth, 8 bits stencil buffer, 4 samples for anti-aliasing, OpenGL version 3.3
    constexpr auto DefaultWindowStyle = sf::Style::Default;
    constexpr auto DefaultWindowState = sf::State::Windowed;
    constexpr auto DefaultFrameRateLimit = 60;
    constexpr auto WindowTitle = "One Gun - SFML";

    int Start(int argc, char *argv[]);
    std::expected<int, std::string> Setup();
    std::expected<int, std::string> Run();
    std::expected<int, std::string> Cleanup();

    sf::Vector2u GetWindowSize();

    void Update();
    void Render();
}
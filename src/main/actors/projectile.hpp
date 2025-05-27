#pragma once

#include <SFML/Graphics/Sprite.hpp>

class Projectile {
public:
    Projectile(const sf::Texture& texture, const sf::Vector2f& startPosition, const sf::Vector2f& direction, float speed);
    Projectile(const Projectile&) = default;
    Projectile(Projectile&&) noexcept = default;
    Projectile& operator=(const Projectile&) = default;
    Projectile& operator=(Projectile&&) noexcept = default;
    ~Projectile() = default;

    void update(float deltaTime);

    [[nodiscard]] const sf::Sprite& getSprite() const { return m_Sprite; }
    [[nodiscard]] bool isActive() const { return m_Active; }

private:
    sf::Sprite m_Sprite; // Projectile's sprite
    sf::Vector2f m_Direction; // Direction of the projectile
    float m_Speed; // Speed of the projectile
    bool m_Active; // Flag to check if the projectile is active

    static constexpr float Lifetime = 5.0f; // Lifetime of the projectile in seconds
    float m_LifeTime; // Current lifetime of the projectile
};
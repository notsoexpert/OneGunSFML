#include <SFML/System/Vector2.hpp>
#include <concepts>

namespace OneGunGame {
template<typename T>
requires std::integral<T>
T ConstrainValueLooped(T value, T min, T max) {
    if (value < min) {
        return max - (min - value) % (max - min);
    } else if (value >= max) {
        return min + (value - max) % (max - min);
    }
    return value;
}

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

template<typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template<Number T1, std::floating_point T2>
constexpr T1 LinearInterpolation(T1 start, T1 end, T2 t) {
    return static_cast<T1>(start * (static_cast<T2>(1) - t) + end * t);
}
template<std::floating_point T2>
constexpr sf::Vector2f LinearInterpolation(const sf::Vector2f& start, const sf::Vector2f& end, T2 t) {
    return start * (static_cast<T2>(1) - t) + end * t;
}
}
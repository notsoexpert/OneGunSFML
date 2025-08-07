#pragma once
#include <SFML/System/Vector2.hpp>
#include <concepts>

namespace OneGunGame {

template<typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template<Number T>
T ConstrainValueLooped(T value, T min, T max) {
    if (value < min) {
        return max - (min - value) % (max - min);
    } else if (value >= max) {
        return min + (value - max) % (max - min);
    }
    return value;
}

template<Number T1, std::floating_point T2>
constexpr T1 LinearInterpolation(T1 start, T1 end, T2 t) {
    return static_cast<T1>(start * (static_cast<T1>(1) - t) + end * t);
}
template<std::floating_point T2>
constexpr sf::Vector2<T2> LinearInterpolation(const sf::Vector2<T2>& start, const sf::Vector2<T2>& end, T2 t) {
    return start * (static_cast<T2>(1) - t) + end * t;
}

template <std::floating_point T>
constexpr float GetDistanceSquared(const sf::Vector2<T>& a, const sf::Vector2<T>& b){
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}
template <Number T1, std::floating_point T2>
constexpr T1 RoundCoordinate(T2 coord) {
    if (coord >= T2{}) {
        return static_cast<T1>(static_cast<long long>(coord + T2{0.5f}));
    }
    return static_cast<T1>(static_cast<long long>(coord - T2{0.5f}));
}
template <std::integral T1, std::floating_point T2>
constexpr sf::Vector2<T1> RoundVector(const sf::Vector2<T2>& vec){
    return sf::Vector2<T1>{RoundCoordinate<T1, T2>(vec.x), RoundCoordinate<T1, T2>(vec.y)};
}

template <std::floating_point T, std::size_t Points>
struct BezierCurve {
    std::array<sf::Vector2<T>, Points> ControlPoints;

    BezierCurve(const std::array<sf::Vector2<T>, Points>& points) :
        ControlPoints(points)
    {}
};

template <std::floating_point T>
constexpr sf::Vector2<T> GetQuadraticCurvePoint(const BezierCurve<T, 3>& curve, float timePoint) {
    return std::pow(1.0f - timePoint, 2.0f) * curve.ControlPoints.at(0) + 
        2.0f * (1.0f - timePoint) * timePoint * curve.ControlPoints.at(1) + 
        std::pow(timePoint, 2.0f) * curve.ControlPoints.at(2);
}
template <std::floating_point T>
constexpr sf::Vector2<T> GetCubicCurvePoint(const BezierCurve<T, 4>& curve, T timePoint) {
    T invPoint = 1.0 - timePoint;
    return invPoint * invPoint * invPoint * curve.ControlPoints.at(0) + 
        T{3.0} * invPoint * invPoint * timePoint * curve.ControlPoints.at(1) + 
        T{3.0} * invPoint * timePoint * timePoint * curve.ControlPoints.at(2) +
        timePoint * timePoint * timePoint * curve.ControlPoints.at(3);
}
}
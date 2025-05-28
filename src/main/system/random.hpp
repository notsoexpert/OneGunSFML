#pragma once

#include <concepts>
#include <random>

namespace Random {
    static std::mt19937 generator; // Mersenne Twister random number generator

    static void Initialize(unsigned int seed) {
        generator.seed(seed);
    }

    template <std::integral T>
    static T GenerateInt(T min, T max) {
        if (min > max) {
            throw std::invalid_argument("Min cannot be greater than Max");
        }
        std::uniform_int_distribution<T> distribution(min, max);
        return distribution(generator);
    }

    // Generate a random float in the range [min, max]
    template <std::floating_point T>
    static T GenerateFloat(T min, T max) {
        if (min > max) {
            throw std::invalid_argument("Min cannot be greater than Max");
        }
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(generator);
    }
    // Generate a random boolean
    // static bool GenerateBool() {
    //     std::bernoulli_distribution distribution(0.5); // 50% chance
    //     return distribution(generator);
    // }
    // // Generate a random choice from a vector
    // template <typename T>
    // static T GenerateChoice(const std::vector<T>& choices) {
    //     if (choices.empty()) {
    //         throw std::invalid_argument("Choices vector cannot be empty");
    //     }
    //     std::uniform_int_distribution<size_t> distribution(0, choices.size() - 1);
    //     return choices[distribution(generator)];
    // }
    // // Generate a random choice from an array
    // template <typename T, size_t N>
    // static T GenerateChoice(const std::array<T, N>& choices) {
    //     if (N == 0) {
    //         throw std::invalid_argument("Choices array cannot be empty");
    //     }
    //     std::uniform_int_distribution<size_t> distribution(0, N - 1);
    //     return choices[distribution(generator)];
    // }

}
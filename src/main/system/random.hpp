#pragma once

#include <concepts>
#include <random>

class Random {
public:
    Random(unsigned int seed) {
        m_Generator.seed(seed);
    }
    
    template <std::integral T>
    T GenerateInt(T min, T max) {
        if (min > max) {
            throw std::invalid_argument("Min cannot be greater than Max");
        }
        std::uniform_int_distribution<T> distribution(min, max);
        return distribution(m_Generator);
    }
    
    template <std::floating_point T>
    T GenerateFloat(T min, T max) {
        if (min > max) { 
            throw std::invalid_argument("Min cannot be greater than Max");
        }
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(m_Generator);
    }

    bool GenerateBool() {
        std::bernoulli_distribution distribution(0.5);
        return distribution(m_Generator);
    }

    template <typename T>
    T GenerateChoice(const std::vector<T>& choices) {
        if (choices.empty()) {
            throw std::invalid_argument("Choices vector cannot be empty");
        }
        std::uniform_int_distribution<size_t> distribution(0, choices.size() - 1);
        return choices[distribution(m_Generator)];
    }

    template <typename T, size_t N>
    T GenerateChoice(const std::array<T, N>& choices) {
        if (N == 0) {
            throw std::invalid_argument("Choices array cannot be empty");
        }
        std::uniform_int_distribution<size_t> distribution(0, N - 1);
        return choices[distribution(m_Generator)];
    }
private:
    std::mt19937 m_Generator; // Mersenne Twister random number generator

};
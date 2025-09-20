#pragma once

#include <random>

namespace core {

class RNG {
public:
    RNG() : m_engine(std::random_device{}()) {}

    void seed(std::uint32_t value) { m_engine.seed(value); }

    float randomFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(m_engine);
    }

    int randomInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(m_engine);
    }

private:
    std::mt19937 m_engine;
};

} // namespace core


#pragma once

#include <chrono>

namespace core {

class TimeStep {
public:
    TimeStep() : m_last(std::chrono::steady_clock::now()) {}

    float tick() {
        const auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> delta = now - m_last;
        m_last = now;
        return delta.count();
    }

private:
    std::chrono::steady_clock::time_point m_last;
};

} // namespace core


#pragma once

#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <vector>

namespace math {

struct Path {
    std::vector<sf::Vector2f> points;
};

inline sf::Vector2f samplePath(const Path& path, int segment, float t) {
    if (path.points.empty()) return {};
    int idx = std::min<int>(segment, static_cast<int>(path.points.size()) - 2);
    float clampedT = std::clamp(t, 0.f, 1.f);
    const auto& a = path.points[idx];
    const auto& b = path.points[idx + 1];
    return sf::Vector2f{a.x + (b.x - a.x) * clampedT, a.y + (b.y - a.y) * clampedT};
}

inline float pathSegmentLength(const Path& path, int segment) {
    if (segment >= static_cast<int>(path.points.size()) - 1) return 0.f;
    sf::Vector2f diff = path.points[segment + 1] - path.points[segment];
    return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

} // namespace math


#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace math {

inline float length(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

inline float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return length(a - b);
}

inline sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = length(v);
    if (len <= 0.0001f) return sf::Vector2f{0.f, 0.f};
    return sf::Vector2f{v.x / len, v.y / len};
}

inline float dot(const sf::Vector2f& a, const sf::Vector2f& b) { return a.x * b.x + a.y * b.y; }

inline sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t) {
    return sf::Vector2f{a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}

inline bool circleCollision(const sf::Vector2f& a, float ra, const sf::Vector2f& b, float rb) {
    return distance(a, b) <= (ra + rb);
}

inline bool rayCircleIntersection(const sf::Vector2f& origin, const sf::Vector2f& dir, float maxDist, const sf::Vector2f& center, float radius) {
    sf::Vector2f toCenter = center - origin;
    float proj = dot(toCenter, dir);
    if (proj < 0.f || proj > maxDist) return false;
    float sqDist = dot(toCenter, toCenter) - proj * proj;
    return sqDist <= radius * radius;
}

} // namespace math


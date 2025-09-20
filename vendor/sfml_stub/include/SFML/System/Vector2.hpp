#pragma once

#include <cmath>

namespace sf {

template <typename T>
struct Vector2 {
    T x{};
    T y{};

    constexpr Vector2() = default;
    constexpr Vector2(T xVal, T yVal) : x(xVal), y(yVal) {}

    template <typename U>
    constexpr explicit Vector2(const Vector2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

    constexpr Vector2<T>& operator+=(const Vector2<T>& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    constexpr Vector2<T>& operator-=(const Vector2<T>& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
};

template <typename T>
constexpr Vector2<T> operator+(Vector2<T> lhs, const Vector2<T>& rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
constexpr Vector2<T> operator-(Vector2<T> lhs, const Vector2<T>& rhs) {
    lhs -= rhs;
    return lhs;
}

template <typename T>
constexpr Vector2<T> operator*(Vector2<T> lhs, T scalar) {
    lhs.x *= scalar;
    lhs.y *= scalar;
    return lhs;
}

template <typename T>
constexpr Vector2<T> operator/(Vector2<T> lhs, T scalar) {
    lhs.x /= scalar;
    lhs.y /= scalar;
    return lhs;
}

template <typename T>
constexpr bool operator==(const Vector2<T>& lhs, const Vector2<T>& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename T>
constexpr bool operator!=(const Vector2<T>& lhs, const Vector2<T>& rhs) {
    return !(lhs == rhs);
}

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;
struct Vector3f {
    float x{};
    float y{};
    float z{};
};


} // namespace sf


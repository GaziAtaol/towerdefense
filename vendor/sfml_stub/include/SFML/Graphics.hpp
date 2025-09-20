#pragma once

#include "System/Vector2.hpp"
#include "Window/Event.hpp"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace sf {

enum PrimitiveType {
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan,
    Quads
};

struct Color {
    std::uint8_t r{0};
    std::uint8_t g{0};
    std::uint8_t b{0};
    std::uint8_t a{255};

    constexpr Color() = default;
    constexpr Color(std::uint8_t rVal, std::uint8_t gVal, std::uint8_t bVal, std::uint8_t aVal = 255)
        : r(rVal), g(gVal), b(bVal), a(aVal) {}

    static const Color White;
    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Transparent;
};

inline const Color Color::White{255, 255, 255, 255};
inline const Color Color::Black{0, 0, 0, 255};
inline const Color Color::Red{255, 0, 0, 255};
inline const Color Color::Green{0, 255, 0, 255};
inline const Color Color::Blue{0, 0, 255, 255};
inline const Color Color::Yellow{255, 255, 0, 255};
inline const Color Color::Transparent{0, 0, 0, 0};

struct FloatRect {
    float left{0.f};
    float top{0.f};
    float width{0.f};
    float height{0.f};

    bool contains(const Vector2f& point) const {
        return point.x >= left && point.x <= left + width && point.y >= top && point.y <= top + height;
    }
};

struct Vector2fBounds {
    Vector2f position{};
    Vector2f size{};
};

class Image {
public:
    void create(unsigned int width, unsigned int height, const Color& color = Color::Transparent) {
        m_width = width;
        m_height = height;
        m_pixels.assign(static_cast<std::size_t>(width) * static_cast<std::size_t>(height), color);
    }

    void setPixel(unsigned int x, unsigned int y, const Color& color) {
        if (x < m_width && y < m_height) {
            m_pixels[static_cast<std::size_t>(y) * m_width + x] = color;
        }
    }

private:
    unsigned int m_width{0};
    unsigned int m_height{0};
    std::vector<Color> m_pixels;
};

class Texture {
public:
    bool loadFromFile(const std::string&) { return true; }
    bool loadFromImage(const Image&) { return true; }
    void setSmooth(bool) {}
    Vector2u getSize() const { return {32u, 32u}; }
};

class Font {
public:
    bool loadFromFile(const std::string&) { return true; }
    static const Font& getDefaultFont() {
        static Font font;
        return font;
    }
};

class Sprite {
public:
    void setColor(const Color&) {}
    void setPosition(const Vector2f&) {}
};

class Text {
public:
    Text() = default;
    Text(const std::string& string, const Font&, unsigned int) : m_string(string) {}

    void setFont(const Font&) {}
    void setString(const std::string& string) { m_string = string; }
    const std::string& getString() const { return m_string; }
    void setCharacterSize(unsigned int) {}
    void setPosition(const Vector2f& pos) { m_position = pos; }
    void setPosition(float x, float y) { m_position = {x, y}; }
    void setFillColor(const Color&) {}

private:
    std::string m_string;
    Vector2f m_position{};
};

class Shape {
public:
    void setFillColor(const Color&) {}
    void setPosition(const Vector2f& pos) { m_position = pos; }
    void setPosition(float x, float y) { m_position = {x, y}; }

protected:
    Vector2f m_position{};
};

class CircleShape : public Shape {
public:
    explicit CircleShape(float radius = 0.f) : m_radius(radius) {}
    void setRadius(float radius) { m_radius = radius; }

private:
    float m_radius;
};

class RectangleShape : public Shape {
public:
    RectangleShape() = default;
    explicit RectangleShape(const Vector2f& size) : m_size(size) {}

    void setSize(const Vector2f& size) { m_size = size; }
    void setOutlineColor(const Color&) {}
    void setOutlineThickness(float) {}

    FloatRect getGlobalBounds() const { return {m_position.x, m_position.y, m_size.x, m_size.y}; }

    void setPosition(const Vector2f& pos) {
        Shape::setPosition(pos);
    }
    void setPosition(float x, float y) {
        setPosition(Vector2f{x, y});
    }

private:
    Vector2f m_size{0.f, 0.f};
};

struct Vertex {
    Vector2f position{};
    Vector2f texCoords{};
    Color color{Color::White};
};

class VertexArray {
public:
    void setPrimitiveType(sf::PrimitiveType) {}
    void resize(std::size_t count) { m_vertices.resize(count); }

    std::size_t getVertexCount() const { return m_vertices.size(); }

    Vertex& operator[](std::size_t index) { return m_vertices[index]; }
    const Vertex& operator[](std::size_t index) const { return m_vertices[index]; }

private:
    std::vector<Vertex> m_vertices;
};

class RenderStates {
public:
    const Texture* texture{nullptr};
};

class VideoMode {
public:
    unsigned int width{0};
    unsigned int height{0};

    VideoMode() = default;
    VideoMode(unsigned int w, unsigned int h) : width(w), height(h) {}
};

class RenderWindow {
public:
    RenderWindow() = default;

    void create(const VideoMode&, const std::string&, unsigned int) { m_open = true; }
    bool isOpen() const { return m_open; }
    void close() { m_open = false; }
    void setFramerateLimit(unsigned int) {}

    bool pollEvent(Event&) { return false; }

    template <typename Drawable>
    void draw(const Drawable&) {}

    void draw(const VertexArray&, const RenderStates&) {}

    void clear(const Color&) {}
    void display() {}

    Vector2f mapPixelToCoords(const Vector2i& position) const { return {static_cast<float>(position.x), static_cast<float>(position.y)}; }

private:
    bool m_open{true};
};

inline Vector2i Mouse::getPosition(const RenderWindow&) { return {0, 0}; }

} // namespace sf


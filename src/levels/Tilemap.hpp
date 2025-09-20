#pragma once

#include "LevelLoader.hpp"
#include <SFML/Graphics.hpp>

namespace levels {

class TilemapRenderer {
public:
    void build(const LevelRuntime& level, const sf::Texture& texture);
    void draw(sf::RenderWindow& window) const;

private:
    sf::VertexArray m_vertices;
    sf::Texture const* m_texture = nullptr;
};

} // namespace levels


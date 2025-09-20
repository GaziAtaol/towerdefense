#include "Tilemap.hpp"

namespace levels {

void TilemapRenderer::build(const LevelRuntime& level, const sf::Texture& texture) {
    m_texture = &texture;
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(level.definition.width * level.definition.height * 4);
    auto tileSize = static_cast<float>(level.definition.tileSize);
    for (int y = 0; y < level.definition.height; ++y) {
        for (int x = 0; x < level.definition.width; ++x) {
            sf::Vertex* quad = &m_vertices[(x + y * level.definition.width) * 4];
            float px = x * tileSize;
            float py = y * tileSize;
            quad[0].position = {px, py};
            quad[1].position = {px + tileSize, py};
            quad[2].position = {px + tileSize, py + tileSize};
            quad[3].position = {px, py + tileSize};
            quad[0].texCoords = {0.f, 0.f};
            quad[1].texCoords = {texture.getSize().x * 1.f, 0.f};
            quad[2].texCoords = {texture.getSize().x * 1.f, texture.getSize().y * 1.f};
            quad[3].texCoords = {0.f, texture.getSize().y * 1.f};
            for (int i = 0; i < 4; ++i) {
                quad[i].color = sf::Color(30, 60, 30);
            }
        }
    }
}

void TilemapRenderer::draw(sf::RenderWindow& window) const {
    if (!m_texture) return;
    sf::RenderStates states;
    states.texture = m_texture;
    window.draw(m_vertices, states);
}

} // namespace levels


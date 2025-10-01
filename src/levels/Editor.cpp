#include "Editor.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

namespace levels {

void LevelEditor::init(int width, int height, int tileSize, const sf::Font& font) {
    m_width = width;
    m_height = height;
    m_tileSize = tileSize;
    m_data.assign(width * height, 0);
    m_hint.setFont(font);
    m_hint.setCharacterSize(16);
    m_hint.setString("Level Editor: Left click toggles path/build tiles, press E to export level.json");
    m_hint.setPosition(40.f, 40.f);
}

void LevelEditor::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        int x = event.mouseButton.x / m_tileSize;
        int y = event.mouseButton.y / m_tileSize;
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            int index = x + y * m_width;
            m_data[index] = (m_data[index] + 1) % 3;
        }
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
        exportJson("editor_export.json");
    }
}

void LevelEditor::draw(sf::RenderWindow& window) const {
    window.draw(m_hint);
    sf::RectangleShape tile({static_cast<float>(m_tileSize - 2), static_cast<float>(m_tileSize - 2)});
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            int value = m_data[x + y * m_width];
            tile.setPosition(static_cast<float>(x * m_tileSize + 1), static_cast<float>(y * m_tileSize + 1));
            if (value == 0) tile.setFillColor(sf::Color(40, 40, 40));
            if (value == 1) tile.setFillColor(sf::Color(100, 80, 20));
            if (value == 2) tile.setFillColor(sf::Color(50, 120, 50));
            window.draw(tile);
        }
    }
}

void LevelEditor::exportJson(const std::string& path) const {
    nlohmann::json::array_t dataArr;
    for (const auto& val : m_data) {
        dataArr.push_back(nlohmann::json(static_cast<nlohmann::json::number_integer_t>(val)));
    }
    
    const nlohmann::json level{{"width", static_cast<nlohmann::json::number_integer_t>(m_width)},
                               {"height", static_cast<nlohmann::json::number_integer_t>(m_height)},
                               {"tileSize", static_cast<nlohmann::json::number_integer_t>(m_tileSize)},
                               {"data", dataArr}};
    std::ofstream file(path);
    file << level.dump(2);
}

} // namespace levels


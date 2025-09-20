#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace levels {

class LevelEditor {
public:
    void init(int width, int height, int tileSize, const sf::Font& font);
    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;
    void exportJson(const std::string& path) const;

private:
    int m_width = 0;
    int m_height = 0;
    int m_tileSize = 40;
    std::vector<int> m_data;
    sf::Text m_hint;
};

} // namespace levels


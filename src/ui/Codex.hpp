#pragma once

#include "../core/GameData.hpp"
#include <SFML/Graphics.hpp>

namespace ui {

class CodexView {
public:
    void init(const sf::Font& font);
    void setDatabase(const data::GameDatabase& database);
    void draw(sf::RenderWindow& window) const;

private:
    std::vector<sf::Text> m_entries;
};

} // namespace ui


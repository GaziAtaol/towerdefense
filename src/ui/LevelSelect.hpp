#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <map>
#include <string>

namespace ui {

class LevelSelect {
public:
    void init(const sf::Font& font);
    void setLevels(const std::map<std::string, std::string>& levels);
    void setOnSelect(std::function<void(const std::string&)> cb);
    void draw(sf::RenderWindow& window) const;
    void handleClick(const sf::Vector2f& point) const;

private:
    std::vector<std::pair<sf::RectangleShape, sf::Text>> m_entries;
    std::function<void(const std::string&)> m_callback;
    const sf::Font* m_font = nullptr;
};

} // namespace ui


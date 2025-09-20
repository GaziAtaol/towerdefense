#include "LevelSelect.hpp"

namespace ui {

void LevelSelect::init(const sf::Font& font) {
    m_font = &font;
    m_entries.clear();
}

void LevelSelect::setLevels(const std::map<std::string, std::string>& levels) {
    m_entries.clear();
    float y = 120.f;
    for (const auto& [id, name] : levels) {
        sf::RectangleShape rect({300.f, 32.f});
        rect.setPosition({80.f, y});
        rect.setFillColor(sf::Color(40, 40, 70));
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(1.f);
        sf::Text text;
        if (m_font) text.setFont(*m_font);
        text.setString(name + " (" + id + ")");
        text.setPosition({90.f, y + 6.f});
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(18);
        m_entries.emplace_back(rect, text);
        y += 40.f;
    }
}

void LevelSelect::setOnSelect(std::function<void(const std::string&)> cb) { m_callback = std::move(cb); }

void LevelSelect::draw(sf::RenderWindow& window) const {
    for (const auto& entry : m_entries) {
        window.draw(entry.first);
        window.draw(entry.second);
    }
}

void LevelSelect::handleClick(const sf::Vector2f& point) const {
    for (const auto& entry : m_entries) {
        if (entry.first.getGlobalBounds().contains(point)) {
            if (m_callback) {
                std::string id = entry.second.getString();
                auto pos = id.find('(');
                if (pos != std::string::npos) {
                    std::string levelId = id.substr(pos + 1);
                    if (!levelId.empty() && levelId.back() == ')') levelId.pop_back();
                    m_callback(levelId);
                }
            }
        }
    }
}

} // namespace ui


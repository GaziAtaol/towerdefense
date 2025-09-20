#include "Codex.hpp"

namespace ui {

void CodexView::init(const sf::Font& font) {
    m_entries.clear();
    sf::Text header;
    header.setFont(font);
    header.setString("Codex");
    header.setCharacterSize(28);
    header.setPosition(420.f, 40.f);
    header.setFillColor(sf::Color::Yellow);
    m_entries.push_back(header);
}

void CodexView::setDatabase(const data::GameDatabase& database) {
    float y = 90.f;
    for (const auto& [id, tower] : database.towers) {
        sf::Text entry = m_entries.front();
        entry.setString("Tower: " + tower.name + " dmg=" + std::to_string(static_cast<int>(tower.damage)) + " range=" + std::to_string(static_cast<int>(tower.range)));
        entry.setPosition(420.f, y);
        entry.setCharacterSize(16);
        entry.setFillColor(sf::Color::White);
        m_entries.push_back(entry);
        y += 20.f;
    }
    y += 20.f;
    for (const auto& [id, enemy] : database.enemies) {
        sf::Text entry = m_entries.front();
        entry.setString("Enemy: " + enemy.name + " hp=" + std::to_string(static_cast<int>(enemy.hp)) + " speed=" + std::to_string(static_cast<int>(enemy.speed)));
        entry.setPosition(420.f, y);
        entry.setCharacterSize(16);
        entry.setFillColor(sf::Color(200, 160, 160));
        m_entries.push_back(entry);
        y += 20.f;
    }
}

void CodexView::draw(sf::RenderWindow& window) const {
    for (const auto& entry : m_entries) {
        window.draw(entry);
    }
}

} // namespace ui


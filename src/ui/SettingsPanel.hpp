#pragma once

#include "../core/GameData.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

namespace ui {

class SettingsPanel {
public:
    void init(const sf::Font& font, const data::SettingsData& settings);
    void draw(sf::RenderWindow& window) const;
    void handleInput(const sf::Event& event);
    const data::SettingsData& settings() const { return m_settings; }

private:
    data::SettingsData m_settings;
    sf::Text m_audio;
    sf::Text m_music;
    sf::Text m_speed;
    sf::Text m_quality;
    sf::Text m_color;
};

} // namespace ui


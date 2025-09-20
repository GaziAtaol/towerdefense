#include "SettingsPanel.hpp"

namespace ui {

void SettingsPanel::init(const sf::Font& font, const data::SettingsData& settings) {
    m_settings = settings;
    m_audio.setFont(font);
    m_music.setFont(font);
    m_speed.setFont(font);
    m_quality.setFont(font);
    m_color.setFont(font);
    m_audio.setPosition(80.f, 150.f);
    m_music.setPosition(80.f, 190.f);
    m_speed.setPosition(80.f, 230.f);
    m_quality.setPosition(80.f, 270.f);
    m_color.setPosition(80.f, 310.f);
    m_audio.setCharacterSize(20);
    m_music.setCharacterSize(20);
    m_speed.setCharacterSize(20);
    m_quality.setCharacterSize(20);
    m_color.setCharacterSize(20);
    m_audio.setFillColor(sf::Color::White);
    m_music.setFillColor(sf::Color::White);
    m_speed.setFillColor(sf::Color::White);
    m_quality.setFillColor(sf::Color::White);
    m_color.setFillColor(sf::Color::White);
    handleInput(sf::Event{});
}

void SettingsPanel::draw(sf::RenderWindow& window) const {
    window.draw(m_audio);
    window.draw(m_music);
    window.draw(m_speed);
    window.draw(m_quality);
    window.draw(m_color);
}

void SettingsPanel::handleInput(const sf::Event& event) {
    (void)event;
    m_audio.setString("Audio Volume: " + std::to_string(static_cast<int>(m_settings.audioVolume * 100)) + "%");
    m_music.setString("Music Volume: " + std::to_string(static_cast<int>(m_settings.musicVolume * 100)) + "%");
    m_speed.setString("Speed: " + std::to_string(m_settings.gameSpeed) + "x");
    m_quality.setString("Graphics: " + m_settings.graphicsQuality);
    m_color.setString("Color mode: " + m_settings.colorBlindMode);
}

} // namespace ui


#include "HUD.hpp"

namespace ui {

void HUD::init(const sf::Font& font) {
    m_lives.setFont(font);
    m_coins.setFont(font);
    m_wave.setFont(font);
    m_speed.setFont(font);
    m_state.setFont(font);
    m_lives.setCharacterSize(18);
    m_coins.setCharacterSize(18);
    m_wave.setCharacterSize(18);
    m_speed.setCharacterSize(18);
    m_state.setCharacterSize(18);
    m_lives.setPosition(10.f, 10.f);
    m_coins.setPosition(10.f, 40.f);
    m_wave.setPosition(10.f, 70.f);
    m_speed.setPosition(10.f, 100.f);
    m_state.setPosition(10.f, 130.f);
}

void HUD::update(int lives, int coins, int wave, float speed, bool paused) {
    m_lives.setString("Lives: " + std::to_string(lives));
    m_coins.setString("Coins: " + std::to_string(coins));
    m_wave.setString("Wave: " + std::to_string(wave));
    m_speed.setString("Speed: " + std::to_string(static_cast<int>(speed)) + "x");
    m_state.setString(paused ? "Paused" : "Running");
}

void HUD::draw(sf::RenderWindow& window) {
    window.draw(m_lives);
    window.draw(m_coins);
    window.draw(m_wave);
    window.draw(m_speed);
    window.draw(m_state);
}

} // namespace ui


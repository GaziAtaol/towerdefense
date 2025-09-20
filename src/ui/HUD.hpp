#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace ui {

class HUD {
public:
    void init(const sf::Font& font);
    void update(int lives, int coins, int wave, float speed, bool paused);
    void draw(sf::RenderWindow& window);

private:
    sf::Text m_lives;
    sf::Text m_coins;
    sf::Text m_wave;
    sf::Text m_speed;
    sf::Text m_state;
};

} // namespace ui


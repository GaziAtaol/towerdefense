#include "Menus.hpp"

namespace ui {

Button::Button(const sf::Font& font, const std::string& label, const sf::Vector2f& position) {
    m_shape.setSize({240.f, 40.f});
    m_shape.setFillColor(sf::Color(60, 60, 90));
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOutlineThickness(2.f);
    m_shape.setPosition(position);
    m_label.setFont(font);
    m_label.setString(label);
    m_label.setCharacterSize(18);
    m_label.setFillColor(sf::Color::White);
    m_label.setPosition(position.x + 10.f, position.y + 8.f);
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
    window.draw(m_label);
}

bool Button::contains(const sf::Vector2f& point) const {
    return m_shape.getGlobalBounds().contains(point);
}

void Button::setCallback(std::function<void()> cb) {
    m_callback = std::move(cb);
}

void Button::trigger() const {
    if (m_callback) m_callback();
}

void MenuScreen::addButton(const Button& button) {
    m_buttons.push_back(button);
}

void MenuScreen::draw(sf::RenderWindow& window) const {
    for (const auto& button : m_buttons) {
        button.draw(window);
    }
}

void MenuScreen::handleClick(const sf::Vector2f& point) const {
    for (const auto& button : m_buttons) {
        if (button.contains(point)) {
            button.trigger();
        }
    }
}

} // namespace ui


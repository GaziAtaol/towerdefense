#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

namespace ui {

class Button {
public:
    Button() = default;
    Button(const sf::Font& font, const std::string& label, const sf::Vector2f& position);
    void draw(sf::RenderWindow& window) const;
    bool contains(const sf::Vector2f& point) const;
    void setCallback(std::function<void()> cb);
    void trigger() const;

private:
    sf::RectangleShape m_shape;
    sf::Text m_label;
    std::function<void()> m_callback;
};

class MenuScreen {
public:
    void addButton(const Button& button);
    void draw(sf::RenderWindow& window) const;
    void handleClick(const sf::Vector2f& point) const;

private:
    std::vector<Button> m_buttons;
};

} // namespace ui


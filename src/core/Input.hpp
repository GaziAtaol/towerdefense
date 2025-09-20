#pragma once

#include <SFML/Window.hpp>
#include <unordered_map>

namespace core {

class InputState {
public:
    void update(const sf::Event& event) {
        if (event.type == sf::Event::MouseMoved) {
            mousePosition.x = event.mouseMove.x;
            mousePosition.y = event.mouseMove.y;
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            mouseButtons[event.mouseButton.button] = true;
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            mouseButtons[event.mouseButton.button] = false;
        }
        if (event.type == sf::Event::KeyPressed) {
            keys[event.key.code] = true;
        }
        if (event.type == sf::Event::KeyReleased) {
            keys[event.key.code] = false;
        }
    }

    bool isMousePressed(sf::Mouse::Button button) const {
        auto it = mouseButtons.find(button);
        return it != mouseButtons.end() && it->second;
    }

    bool isKeyPressed(sf::Keyboard::Key key) const {
        auto it = keys.find(key);
        return it != keys.end() && it->second;
    }

    sf::Vector2i mousePosition{0, 0};

private:
    std::unordered_map<int, bool> mouseButtons;
    std::unordered_map<int, bool> keys;
};

} // namespace core


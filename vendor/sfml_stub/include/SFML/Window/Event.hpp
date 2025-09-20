#pragma once

#include <cstdint>

#include "../System/Vector2.hpp"

namespace sf {

class RenderWindow;

class Keyboard {
public:
    enum Key {
        Unknown = -1,
        Escape,
        P,
        Num1,
        Num2,
        Num3,
        E
    };
};

class Mouse {
public:
    enum Button {
        Left,
        Right,
        Middle
    };

    static Vector2i getPosition(const RenderWindow& window);
};

struct Event {
    enum EventType {
        Closed,
        Resized,
        LostFocus,
        GainedFocus,
        TextEntered,
        KeyPressed,
        KeyReleased,
        MouseWheelMoved,
        MouseWheelScrolled,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseEntered,
        MouseLeft
    } type;

    struct MouseButtonEvent {
        int x{};
        int y{};
        int button{};
    } mouseButton;

    struct MouseMoveEvent {
        int x{};
        int y{};
    } mouseMove;

    struct KeyEvent {
        Keyboard::Key code{Keyboard::Unknown};
    } key;
};

namespace Style {
    constexpr unsigned Default = 0u;
}

} // namespace sf


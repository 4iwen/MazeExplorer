#pragma once

#include "platform.h"

#include <array>

enum class Key {
#ifdef PLATFORM_GLFW
    A = GLFW_KEY_A,
    R = GLFW_KEY_R,
    W = GLFW_KEY_W,
    S = GLFW_KEY_S,
    D = GLFW_KEY_D,
    ESCAPE = GLFW_KEY_ESCAPE,
    UP = GLFW_KEY_UP,
    DOWN = GLFW_KEY_DOWN,
    LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
    COUNT = GLFW_KEY_LAST + 1
#endif
};

enum class Mouse_Button {
#ifdef PLATFORM_GLFW
    LEFT = GLFW_MOUSE_BUTTON_LEFT,
    RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
    MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
    COUNT = GLFW_MOUSE_BUTTON_LAST + 1
#endif
};

class Input {
public:
    static void poll();

    static bool is_key_down(Key key);

    static bool is_key_pressed(Key key);

    static bool is_key_released(Key key);

    static bool is_mouse_button_pressed(Mouse_Button button);

    static bool is_mouse_captured();

    static double get_mouse_dx() { return s_mouse_delta_x; }
    static double get_mouse_dy() { return s_mouse_delta_y; }

    static void on_key(int key, int action);

    static void on_cursor(double x, double y);

    static void on_mouse_button(int button, int action);

private:
    // This is not ideal, but since we're working with a single window its fine for a prototype.
    static std::array<bool, static_cast<size_t>(Key::COUNT)> s_keys_current;
    static std::array<bool, static_cast<size_t>(Key::COUNT)> s_keys_previous;

    static std::array<bool, static_cast<size_t>(Mouse_Button::COUNT)> s_mouse_current;
    static std::array<bool, static_cast<size_t>(Mouse_Button::COUNT)> s_mouse_previous;

    static double s_mouse_x;
    static double s_mouse_y;
    static double s_mouse_delta_x;
    static double s_mouse_delta_y;
    static bool s_first_mouse_event;
    static bool s_mouse_captured;
};

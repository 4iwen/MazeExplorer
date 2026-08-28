#pragma once

#include "platform.h"

#include <array>

enum class Key {
#ifdef PLATFORM_GLFW
    A = GLFW_KEY_A,
    W = GLFW_KEY_W,
    S = GLFW_KEY_S,
    D = GLFW_KEY_D,
    ESCAPE = GLFW_KEY_ESCAPE,
    COUNT = GLFW_KEY_LAST + 1
#endif
};

class Input {
public:
    static void poll();

    static bool is_key_down(Key key);

    static bool is_key_pressed(Key key);

    static bool is_key_released(Key key);

    static double get_mouse_dx() { return s_mouse_delta_x; }
    static double get_mouse_dy() { return s_mouse_delta_y; }

    static void on_key(int key, int action);

    static void on_cursor(double x, double y);

private:
#ifdef PLATFORM_GLFW
    friend void key_callback(GLFWwindow *, int, int, int, int);

    friend void cursor_callback(GLFWwindow *, double, double);
#endif

    static std::array<bool, static_cast<size_t>(Key::COUNT)> s_keys_current;
    static std::array<bool, static_cast<size_t>(Key::COUNT)> s_keys_previous;

    static double s_mouse_x;
    static double s_mouse_y;
    static double s_mouse_delta_x;
    static double s_mouse_delta_y;
    static bool s_first_mouse_event;
};

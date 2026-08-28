#include "input.h"

std::array<bool, static_cast<size_t>(Key::COUNT)> Input::s_keys_current{};
std::array<bool, static_cast<size_t>(Key::COUNT)> Input::s_keys_previous{};
double Input::s_mouse_x = 0.0;
double Input::s_mouse_y = 0.0;
double Input::s_mouse_delta_x = 0.0;
double Input::s_mouse_delta_y = 0.0;
bool Input::s_first_mouse_event = true;

void Input::poll() {
    s_keys_previous = s_keys_current;
    s_mouse_delta_x = 0.0;
    s_mouse_delta_y = 0.0;
}

bool Input::is_key_down(Key key) {
    return s_keys_current[static_cast<size_t>(key)];
}

bool Input::is_key_pressed(Key key) {
    auto i = static_cast<size_t>(key);
    return s_keys_current[i] && !s_keys_previous[i];
}

bool Input::is_key_released(Key key) {
    auto i = static_cast<size_t>(key);
    return !s_keys_current[i] && s_keys_previous[i];
}

void Input::on_key(int key, int action) {
    if (key < 0 || key >= static_cast<int32_t>(Key::COUNT)) {
        return;
    }

#ifdef PLATFORM_GLFW
    if (action == GLFW_PRESS) {
        s_keys_current[key] = true;
    }
    if (action == GLFW_RELEASE) {
        s_keys_current[key] = false;
    }
#endif
}

void Input::on_cursor(double x, double y) {
    if (s_first_mouse_event) {
        s_mouse_x = x;
        s_mouse_y = y;
        s_first_mouse_event = false;
    }

    s_mouse_delta_x += x - s_mouse_x;
    s_mouse_delta_y += y - s_mouse_y;
    s_mouse_x = x;
    s_mouse_y = y;
}

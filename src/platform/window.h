#pragma once

#include "platform.h"

#include <string>
#include <functional>

#include "input.h"

class Window {
public:
    Window(
        uint32_t width,
        uint32_t height,
        const std::string &title
    );

    ~Window();

    Window(const Window &other) = delete;

    Window &operator=(const Window &other) = delete;

    bool should_close();

    void present();

    void poll_events();

    void set_vsync(bool vsync);

    void set_cursor_locked(bool locked);

    using Resize_Callback = std::function<void(int32_t, int32_t)>;
    using Key_Callback = std::function<void(int32_t, int32_t, int32_t, int32_t)>;
    using Cursor_Callback = std::function<void(double, double)>;
    using Mouse_Button_Callback = std::function<void(int32_t, int32_t, int32_t)>;

    void set_resize_callback(Resize_Callback callback) { m_resize_callback = std::move(callback); }
    void set_key_callback(Key_Callback callback) { m_key_callback = std::move(callback); }
    void set_cursor_callback(Cursor_Callback callback) { m_cursor_callback = std::move(callback); }
    void set_mouse_button_callback(Mouse_Button_Callback callback) { m_mouse_button_callback = std::move(callback); }

private:
#ifdef PLATFORM_GLFW
    GLFWwindow *m_handle;

    friend void framebuffer_size_callback(GLFWwindow *, int, int);

    friend void key_callback(GLFWwindow *, int, int, int, int);

    friend void cursor_callback(GLFWwindow *, double, double);

    friend void mouse_button_callback(GLFWwindow *, int, int, int);
#endif

    uint32_t m_width;
    uint32_t m_height;

    Resize_Callback m_resize_callback;
    Key_Callback m_key_callback;
    Cursor_Callback m_cursor_callback;
    Mouse_Button_Callback m_mouse_button_callback;
};

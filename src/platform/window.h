#pragma once

#include "platform.h"

#include <string>
#include <functional>

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

    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }

    using Resize_Callback = std::function<void(uint32_t, uint32_t)>;

    void set_resize_callback(Resize_Callback callback);

private:
#ifdef PLATFORM_GLFW
    GLFWwindow *m_handle;
#endif

    uint32_t m_width;
    uint32_t m_height;

    friend void framebuffer_size_callback(GLFWwindow *, int, int);

    void on_resize(uint32_t width, uint32_t height);

    Resize_Callback m_resize_callback;
};

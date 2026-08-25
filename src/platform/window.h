#pragma once

#include "platform.h"

#include <string>

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

private:
    void *m_handle;
};

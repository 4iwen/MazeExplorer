#include "window.h"

#include <iostream>

#ifdef PLATFORM_GLFW
static void error_callback(int error, const char* description) {
    std::cout << "Error (" << error << "): " << description << std::endl;
}
#endif

Window::Window(
    uint32_t width,
    uint32_t height,
    const std::string &title
) {
#ifdef PLATFORM_GLFW
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_handle = glfwCreateWindow(
        static_cast<int32_t>(width),
        static_cast<int32_t>(height),
        title.c_str(),
        nullptr, nullptr
    );

    if (!m_handle) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(m_handle);
#endif
}

Window::~Window() {
#ifdef PLATFORM_GLFW
    glfwDestroyWindow(m_handle);
    glfwTerminate();
#endif
}

bool Window::should_close() {
#ifdef PLATFORM_GLFW
    return glfwWindowShouldClose(m_handle);
#endif
}

void Window::present() {
#ifdef PLATFORM_GLFW
    glfwSwapBuffers(m_handle);
    glfwPollEvents();
#endif
}

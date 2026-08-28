#include "window.h"

#include <iostream>

#ifdef PLATFORM_GLFW
static void error_callback(int error, const char *description) {
    std::cout << "Error (" << error << "): " << description << std::endl;
}

static void framebuffer_size_callback(
    GLFWwindow *handle,
    int width,
    int height
) {
    auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

    window->m_width = width;
    window->m_height = height;
    window->m_resize_callback(width, height);
}

static void key_callback(
    GLFWwindow *handle,
    int key,
    int scancode,
    int action,
    int mods
) {
    auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

    window->m_key_callback(key, scancode, action, mods);
}

static void cursor_callback(
    GLFWwindow *handle,
    double x,
    double y
) {
    auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

    window->m_cursor_callback(x, y);
}

Window::Window(
    uint32_t width,
    uint32_t height,
    const std::string &title
) : m_width(width), m_height(height) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
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

    glfwSetWindowUserPointer(m_handle, this);

    glfwSetFramebufferSizeCallback(m_handle, framebuffer_size_callback);
    glfwSetKeyCallback(m_handle, key_callback);
    glfwSetCursorPosCallback(m_handle, cursor_callback);

    glfwMakeContextCurrent(m_handle);
}

Window::~Window() {
    glfwDestroyWindow(m_handle);
    glfwTerminate();
}

bool Window::should_close() {
    return glfwWindowShouldClose(m_handle);
}

void Window::present() {
    glfwSwapBuffers(m_handle);
}

void Window::poll_events() {
    glfwPollEvents();
}

void Window::set_vsync(bool vsync) {
    glfwSwapInterval(vsync ? 1 : 0);
}

void Window::set_cursor_locked(bool locked) {
    glfwSetInputMode(
        m_handle,
        GLFW_CURSOR,
        locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
    );
}
#endif

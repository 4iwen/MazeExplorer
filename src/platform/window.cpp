#include "window.h"

#include <iostream>

#ifdef PLATFORM_GLFW
static void error_callback(int error, const char* description) {
    std::cout << "Error (" << error << "): " << description << std::endl;
}

static void framebuffer_size_callback(
    GLFWwindow* handle,
    int width,
    int height
) {
    auto* window = static_cast<Window*>(
        glfwGetWindowUserPointer(handle)
    );

    window->on_resize(
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    );
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

    glfwSetFramebufferSizeCallback(
        m_handle,
        framebuffer_size_callback
    );

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
    glfwPollEvents();
}
#endif

void Window::set_resize_callback(Resize_Callback callback) {
    m_resize_callback = std::move(callback);
}

void Window::on_resize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;

    m_resize_callback(width, height);
}

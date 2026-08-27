#pragma once

#ifdef PLATFORM_GLFW
#include <GLFW/glfw3.h>

inline void *get_gl_proc_address(const char* name) {
    return reinterpret_cast<void*>(glfwGetProcAddress(name));
}

#else
#error "No platform backend option provided."
#endif
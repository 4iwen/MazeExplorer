#include "time.h"

double Time::elapsed() {
#ifdef PLATFORM_BACKEND_GLFW
    return glfwGetTime();
#endif
}

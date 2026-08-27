#include "time.h"

double Time::elapsed() {
#ifdef PLATFORM_GLFW
    return glfwGetTime();
#endif
}

#pragma once

#ifdef PLATFORM_BACKEND_GLFW
#include <GLFW/glfw3.h>
#else
#error "No platform backend option provided."
#endif
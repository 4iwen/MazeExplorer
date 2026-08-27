#pragma once

#ifdef RENDERER_OPENGL
#include <glad/gl.h>
#else
#error "No renderer backend option provided."
#endif
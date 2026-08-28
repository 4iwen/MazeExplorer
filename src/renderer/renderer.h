#pragma once

#include "renderer_backend.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"

#include <glm/glm.hpp>
#include <cstdint>

class Renderer {
public:
    static void initialize();

    static void begin_scene(const Camera &camera);

    static void end_scene();

    static void draw(
        const Mesh &mesh,
        const Shader &shader
    );

    static void set_viewport(uint32_t width, uint32_t height);

private:
    static void clear(glm::vec4 color);

#ifdef RENDERER_OPENGL
    static void initialize_opengl();
#endif

    static glm::mat4 s_view_projection_matrix;
};

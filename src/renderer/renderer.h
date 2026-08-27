#pragma once

#include "renderer_backend.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"

#include <glm/glm.hpp>
#include <cstdint>

class Renderer {
public:
    Renderer();

    static void begin_scene(const Camera &camera);

    static void end_scene();

    static void draw(
        const Mesh &mesh,
        const Shader &shader
    );

private:
    static void clear(glm::vec4 color);

#ifdef RENDERER_OPENGL
    void initialize_opengl();
#endif

    uint32_t m_vbo;
    uint32_t m_ebo;
    uint32_t m_vao;
};

#pragma once

#include "renderer_backend.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "texture2d.h"

#include <glm/glm.hpp>
#include <cstdint>

class Renderer {
public:
    static void initialize();

    static void begin_scene(const Camera &camera);

    static void end_scene();

    static void begin_ui(uint32_t screen_width, uint32_t screen_height);

    static void end_ui();

    static void draw(
        const Mesh &mesh,
        const Shader &shader,
        const glm::mat4 &transform = glm::mat4(1.0f)
    );

    static void draw_ui(
        const Mesh& mesh,
        const Shader& shader,
        const Texture2D& texture,
        glm::vec2 position,
        glm::vec2 size
    );

    static void draw_ui_quad(
        const Mesh& mesh,
        const Shader& shader,
        glm::vec2 position,
        glm::vec2 size,
        glm::vec4 color
    );

    static void set_viewport(uint32_t width, uint32_t height);

    static void set_depth_test(bool enable);

private:
    static void clear(glm::vec4 color);

#ifdef RENDERER_OPENGL
    static void initialize_opengl();
#endif

    static glm::mat4 s_view_projection_matrix;
};

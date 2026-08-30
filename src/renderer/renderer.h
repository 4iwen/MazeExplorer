#pragma once

#include "renderer_backend.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "lighting.h"
#include "material.h"

#include <glm/glm.hpp>
#include <cstdint>
#include <span>
#include <vector>

#include "render_command.h"

class Renderable;

class Renderer {
public:
    static void initialize();

    static void begin_frame(
        const Camera &camera,
        const Lighting &lighting,
        uint32_t screen_width,
        uint32_t screen_height
    );

    static void end_frame();

    static void submit(const Renderable &renderable);
    static void submit(std::span<const Renderable> renderables);

    static void set_viewport(uint32_t width, uint32_t height);

    static void set_depth_test(bool enable);

private:
    static void draw_command(const Render_Command &command);

    static void apply_lighting(const Material &material);

    static void clear(glm::vec4 color);

#ifdef RENDERER_OPENGL
    static void initialize_opengl();
#endif

    static glm::mat4 s_view_matrix;
    static glm::mat4 s_projection_matrix;
    static glm::mat4 s_ui_view_projection_matrix;
    static glm::vec3 s_view_position;
    static Lighting s_lighting;

    static std::vector<Render_Command> s_render_queue;
};

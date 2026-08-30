#include "renderer.h"

#include <stdexcept>
#include <string>
#include <algorithm>
#include <vector>

#include "platform/platform.h"
#include "renderable.h"

glm::mat4 Renderer::s_view_matrix = glm::mat4(1.0f);
glm::mat4 Renderer::s_projection_matrix = glm::mat4(1.0f);
glm::mat4 Renderer::s_ui_view_projection_matrix = glm::mat4(1.0f);
glm::vec3 Renderer::s_view_position{0.0f};
Lighting Renderer::s_lighting{};
std::vector<Render_Command> Renderer::s_render_queue{};

void Renderer::initialize() {
#ifdef RENDERER_OPENGL
    initialize_opengl();
#endif
}

void Renderer::begin_frame(const Camera &camera, const Lighting &lighting, uint32_t screen_width, uint32_t screen_height) {
    s_view_matrix = camera.get_view_matrix();
    s_projection_matrix = camera.get_projection_matrix();
    s_view_position = camera.get_position();
    s_lighting = lighting;
    s_ui_view_projection_matrix = glm::ortho(
        0.0f,
        static_cast<float>(screen_width),
        static_cast<float>(screen_height),
        0.0f,
        -1.0f,
        1.0f
    );
    s_render_queue.clear();
    clear({0.33f, 0.56f, 0.71f, 1.0f});
}

void Renderer::end_frame() {
    // Reorder world geometry
    auto world_end = std::partition(s_render_queue.begin(), s_render_queue.end(), [](const Render_Command &command) {
        return command.pass == Render_Pass::WORLD;
    });

    std::sort(s_render_queue.begin(), world_end, [](const Render_Command &left, const Render_Command &right) {
        if (left.material->get_sort_key() != right.material->get_sort_key()) {
            return std::less<const void *>{}(left.material->get_sort_key(), right.material->get_sort_key());
        }
        if (left.material != right.material) {
            return std::less<const Material *>{}(left.material, right.material);
        }
        return std::less<const Mesh *>{}(left.mesh, right.mesh);
    });

    // Draw world geometry with depth testing
    set_depth_test(true);
    for (auto command = s_render_queue.begin(); command != world_end; ++command) {
        draw_command(*command);
    }

    // UI layers must preserve submission order when they share the same layer
    std::stable_sort(world_end, s_render_queue.end(), [](const Render_Command &left, const Render_Command &right) {
        return left.layer < right.layer;
    });

    // UI is drawn over the world
    set_depth_test(false);
    for (auto command = world_end; command != s_render_queue.end(); ++command) {
        draw_command(*command);
    }

    set_depth_test(true);
}

void Renderer::submit(const Renderable &renderable) {
    s_render_queue.push_back({
        .mesh = &renderable.get_mesh(),
        .material = &renderable.get_material(),
        .transform = renderable.get_transform(),
        .pass = renderable.get_pass(),
        .layer = renderable.get_layer()
    });
}

void Renderer::submit(std::span<const Renderable> renderables) {
    for (const Renderable &renderable : renderables) {
        submit(renderable);
    }
}

void Renderer::draw_command(const Render_Command &command) {
    const Mesh &mesh = *command.mesh;
    const Material &material = *command.material;
    material.bind();
    if (command.pass == Render_Pass::WORLD) {
        material.set_mat4("u_model", command.transform);
        material.set_mat4("u_view", s_view_matrix);
        material.set_mat4("u_projection", s_projection_matrix);
        material.set_int("u_albedo_texture", 0);
        apply_lighting(material);
    } else {
        material.set_mat4("view_projection_matrix", s_ui_view_projection_matrix);
        material.set_mat4("model_matrix", command.transform);
        if (material.has_albedo()) {
            material.set_int("texture_sampler", 0);
        } else {
            material.set_vec4("color", material.get_color());
        }
    }
    mesh.draw();
}

void Renderer::apply_lighting(const Material &material) {
    material.set_vec3("u_view_position", s_view_position);
    material.set_vec3("u_global_ambient", s_lighting.global_ambient);
    material.set_int("u_point_light_count", static_cast<int>(s_lighting.point_light_count));
    material.set_int("u_spot_light_count", static_cast<int>(s_lighting.spot_light_count));

    for (uint32_t i = 0; i < s_lighting.point_light_count; ++i) {
        const Point_Light &light = s_lighting.point_lights[i];
        const std::string prefix = "u_point_lights[" + std::to_string(i) + "].";

        material.set_vec3(prefix + "position", light.position);
        material.set_vec3(prefix + "ambient", light.ambient);
        material.set_vec3(prefix + "diffuse", light.diffuse);
        material.set_vec3(prefix + "specular", light.specular);
        material.set_float(prefix + "constant", light.constant);
        material.set_float(prefix + "linear", light.linear);
        material.set_float(prefix + "quadratic", light.quadratic);
    }

    for (uint32_t i = 0; i < s_lighting.spot_light_count; ++i) {
        const Spot_Light &light = s_lighting.spot_lights[i];
        const std::string prefix = "u_spot_lights[" + std::to_string(i) + "].";

        material.set_vec3(prefix + "position", light.position);
        material.set_vec3(prefix + "direction", light.direction);
        material.set_vec3(prefix + "ambient", light.ambient);
        material.set_vec3(prefix + "diffuse", light.diffuse);
        material.set_vec3(prefix + "specular", light.specular);
        material.set_float(prefix + "cutoff", light.cutoff);
        material.set_float(prefix + "outer_cutoff", light.outer_cutoff);
        material.set_float(prefix + "constant", light.constant);
        material.set_float(prefix + "linear", light.linear);
        material.set_float(prefix + "quadratic", light.quadratic);
    }
}

#ifdef RENDERER_OPENGL
void Renderer::set_viewport(uint32_t width, uint32_t height) {
    glViewport(
        0,
        0,
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height)
    );
}

void Renderer::set_depth_test(bool enable) {
#ifdef RENDERER_OPENGL
    if (enable) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
#endif
}

void Renderer::clear(glm::vec4 color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::initialize_opengl() {
    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(get_gl_proc_address))) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    glEnable(GL_DEPTH_TEST);
}
#endif

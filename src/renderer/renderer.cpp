#include "renderer.h"

#include "platform/platform.h"

#include <stdexcept>
#include <string>

glm::mat4 Renderer::s_view_projection_matrix = glm::mat4(1.0f);
glm::mat4 Renderer::s_view_matrix = glm::mat4(1.0f);
glm::mat4 Renderer::s_projection_matrix = glm::mat4(1.0f);
glm::vec3 Renderer::s_view_position{0.0f};
Lighting Renderer::s_lighting{};

void Renderer::initialize() {
#ifdef RENDERER_OPENGL
    initialize_opengl();
#endif
}

void Renderer::begin_scene(const Camera &camera, const Lighting &lighting) {
    s_view_matrix = camera.get_view_matrix();
    s_projection_matrix = camera.get_projection_matrix();
    s_view_projection_matrix = s_projection_matrix * s_view_matrix;
    s_view_position = camera.get_position();
    s_lighting = lighting;
    clear({0.1f, 0.1f, 0.1f, 1.0f});
}

void Renderer::end_scene() {
}

void Renderer::begin_ui(uint32_t screen_width, uint32_t screen_height) {
    s_view_projection_matrix = glm::ortho(
        0.0f,
        static_cast<float>(screen_width),
        static_cast<float>(screen_height),
        0.0f,
        -1.0f,
        1.0f
    );


    set_depth_test(false);
}


void Renderer::end_ui() {
    set_depth_test(true);
}

void Renderer::draw(
    const Mesh &mesh,
    const Shader &shader,
    const glm::mat4 &transform
) {
    shader.use();
    shader.set_mat4("u_model", transform);
    shader.set_mat4("u_view", s_view_matrix);
    shader.set_mat4("u_projection", s_projection_matrix);
    apply_lighting(shader);
    mesh.draw();
}

void Renderer::apply_lighting(const Shader &shader) {
    shader.set_vec3("u_view_position", s_view_position);
    shader.set_vec3("u_global_ambient", s_lighting.global_ambient);
    shader.set_int("u_point_light_count", static_cast<int>(s_lighting.point_light_count));
    shader.set_int("u_spot_light_count", static_cast<int>(s_lighting.spot_light_count));

    for (uint32_t i = 0; i < s_lighting.point_light_count; ++i) {
        const Point_Light &light = s_lighting.point_lights[i];
        const std::string prefix = "u_point_lights[" + std::to_string(i) + "].";

        shader.set_vec3(prefix + "position", light.position);
        shader.set_vec3(prefix + "ambient", light.ambient);
        shader.set_vec3(prefix + "diffuse", light.diffuse);
        shader.set_vec3(prefix + "specular", light.specular);
        shader.set_float(prefix + "constant", light.constant);
        shader.set_float(prefix + "linear", light.linear);
        shader.set_float(prefix + "quadratic", light.quadratic);
    }

    for (uint32_t i = 0; i < s_lighting.spot_light_count; ++i) {
        const Spot_Light &light = s_lighting.spot_lights[i];
        const std::string prefix = "u_spot_lights[" + std::to_string(i) + "].";

        shader.set_vec3(prefix + "position", light.position);
        shader.set_vec3(prefix + "direction", light.direction);
        shader.set_vec3(prefix + "ambient", light.ambient);
        shader.set_vec3(prefix + "diffuse", light.diffuse);
        shader.set_vec3(prefix + "specular", light.specular);
        shader.set_float(prefix + "cutoff", light.cutoff);
        shader.set_float(prefix + "outer_cutoff", light.outer_cutoff);
        shader.set_float(prefix + "constant", light.constant);
        shader.set_float(prefix + "linear", light.linear);
        shader.set_float(prefix + "quadratic", light.quadratic);
    }
}

void Renderer::draw_ui(
    const Mesh &mesh,
    const Shader &shader,
    const Texture2D &texture,
    glm::vec2 position,
    glm::vec2 size
) {
    glm::mat4 transform{1.0f};

    transform = glm::translate(
        transform,
        glm::vec3(
            position.x + size.x * 0.5f,
            position.y + size.y * 0.5f,
            0.0f
        )
    );

    transform = glm::scale(
        transform,
        glm::vec3(
            size.x,
            size.y,
            1.0f
        )
    );

    texture.bind(0);
    shader.use();
    shader.set_int("texture_sampler", 0);
    shader.set_mat4("view_projection_matrix", s_view_projection_matrix);
    shader.set_mat4("model_matrix", transform);
    mesh.draw();
}

void Renderer::draw_ui_quad(
    const Mesh &mesh,
    const Shader &shader,
    glm::vec2 position,
    glm::vec2 size,
    glm::vec4 color
) {
    glm::mat4 transform{1.0f};

    transform = glm::translate(
        transform,
        glm::vec3(
            position.x + size.x * 0.5f,
            position.y + size.y * 0.5f,
            0.0f
        )
    );

    transform = glm::scale(
        transform,
        glm::vec3(
            size.x,
            size.y,
            1.0f
        )
    );

    shader.use();
    shader.set_vec4("color", color);
    shader.set_mat4("view_projection_matrix", s_view_projection_matrix);
    shader.set_mat4("model_matrix", transform);
    mesh.draw();
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

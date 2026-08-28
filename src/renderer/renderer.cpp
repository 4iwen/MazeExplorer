#include "renderer.h"

#include "platform/platform.h"

#include <stdexcept>

glm::mat4 Renderer::s_view_projection_matrix = glm::mat4(1.0f);

void Renderer::initialize() {
#ifdef RENDERER_OPENGL
    initialize_opengl();
#endif
}

void Renderer::begin_scene(const Camera &camera) {
    s_view_projection_matrix = camera.get_projection_matrix() * camera.get_view_matrix();
    clear({0.1f, 0.1f, 0.1f, 1.0f});
}

void Renderer::end_scene() {
}

void Renderer::draw(
    const Mesh &mesh,
    const Shader &shader
) {
    shader.use();
    shader.set_mat4("view_projection_matrix", s_view_projection_matrix);
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

void Renderer::clear(glm::vec4 color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::initialize_opengl() {
    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(get_gl_proc_address))) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }
}
#endif

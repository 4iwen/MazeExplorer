#include "renderer.h"

#include "platform/platform.h"

#include <stdexcept>

Renderer::Renderer() {
#ifdef RENDERER_OPENGL
    initialize_opengl();
#endif
}

void Renderer::begin_scene(const Camera &camera) {
    clear({0.1f, 0.1f, 0.1f, 1.0f});
}

void Renderer::end_scene() {
}

void Renderer::draw(
    const Mesh &mesh,
    const Shader &shader
) {
    shader.use();
    mesh.draw();
}

#ifdef RENDERER_OPENGL
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

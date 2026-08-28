#include "game.h"

#include "renderer/renderer.h"
#include "platform/time.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string read_entire_file(const std::string &path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Game::Game(uint32_t window_width, uint32_t window_height)
    : m_window_width(window_width),
      m_window_height(window_height),
      m_quad_mesh(Mesh::quad()),
      m_quad_shader(
          read_entire_file("assets/shaders/quad.vert"),
          read_entire_file("assets/shaders/quad.frag")
      ),
      m_camera(
          70,
          static_cast<float>(m_window_width) / static_cast<float>(m_window_height),
          {0, 0, 1}
      ) {
}

Game::~Game() = default;

// called periodically
void Game::fixed_update(double delta) {
}

// called on every frame
void Game::update(double delta) {
    m_camera.set_rotation(
        {0.0f, static_cast<float>(Time::elapsed()) * 0.5f, 0.0f}
    );

    Renderer::begin_scene(m_camera);

    Renderer::draw(m_quad_mesh, m_quad_shader);

    Renderer::end_scene();
}

void Game::on_resize(uint32_t width, uint32_t height) {
    m_window_width = width;
    m_window_height = height;

    m_camera.set_aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
}

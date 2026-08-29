#include "game.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "renderer/renderer.h"
#include "platform/time.h"
#include "platform/input.h"

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
      m_player(
          {0, 0, 1},
          70,
          static_cast<float>(m_window_width) / static_cast<float>(m_window_height)
      ) {
}

Game::~Game() = default;

// called periodically
void Game::fixed_update(double delta) {
}

// called on every frame
void Game::update(double delta) {
    float deltaf = static_cast<float>(delta);
    m_player.update(deltaf);

    Renderer::begin_scene(m_player.get_camera());

    Renderer::draw(m_quad_mesh, m_quad_shader);

    Renderer::end_scene();
}

void Game::on_resize(uint32_t width, uint32_t height) {
    m_window_width = width;
    m_window_height = height;

    m_player.get_camera().set_aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
    //m_camera.set_aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
}

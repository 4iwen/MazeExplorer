#include "game.h"

#include "renderer/renderer.h"

#include <fstream>
#include <sstream>

std::string read_entire_file(const std::string &path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Game::Game()
    : m_quad_mesh(Mesh::quad()),
      m_quad_shader(
          read_entire_file("assets/shaders/quad.vert"),
          read_entire_file("assets/shaders/quad.frag")
      ),
      m_camera(70, 1152.0 / 720.0) { // TODO: Get current window width and height
}

Game::~Game() = default;

// called periodically
void Game::fixed_update(double delta) {
}

// called on every frame
void Game::update(double delta) {
    Renderer::begin_scene(m_camera);

    Renderer::draw(m_quad_mesh, m_quad_shader);

    Renderer::end_scene();
}

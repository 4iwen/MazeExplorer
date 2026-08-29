#include "game.h"

#include "maze_generator.h"
#include "renderer/renderer.h"
#include "platform/time.h"
#include "platform/input.h"
#include "utils/utils.h"

Game::Game(uint32_t window_width, uint32_t window_height)
    : m_window_width(window_width),
      m_window_height(window_height),

      m_quad_mesh(Mesh::quad()),
      m_quad_shader(
          Utils::read_entire_file("assets/shaders/quad.vert"),
          Utils::read_entire_file("assets/shaders/quad.frag")
      ),

      m_player(
          {0, 0, 1},
          70,
          static_cast<float>(m_window_width) / static_cast<float>(m_window_height)
      ),
      m_maze(Maze_Generator::generate(21, 21)),
      m_maze_texture(m_maze.to_texture2D()) {
}

Game::~Game() = default;

// called periodically
void Game::fixed_update(double delta) {
}

// called on every frame
void Game::update(double delta) {
    float deltaf = static_cast<float>(delta);
    m_player.update(deltaf);

    m_quad_shader.use();
    m_quad_shader.set_int("texture_sampler", 0);
    m_maze_texture.bind(0);

    Renderer::draw(
        m_quad_mesh,
        m_quad_shader
    );

    Renderer::begin_scene(m_player.get_camera());

    Renderer::draw(m_quad_mesh, m_quad_shader);

    Renderer::end_scene();
}

void Game::on_resize(uint32_t width, uint32_t height) {
    m_window_width = width;
    m_window_height = height;

    m_player.get_camera().set_aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
}

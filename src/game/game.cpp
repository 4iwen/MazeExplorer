#include "game.h"

#include "maze_generator.h"
#include "renderer/renderer.h"
#include "platform/time.h"
#include "platform/input.h"

Game::Game(uint32_t window_width, uint32_t window_height)
    : m_window_width(window_width),
      m_window_height(window_height),
      m_maze(Maze_Generator::generate(31, 31)),
      m_minimap(m_maze),
      m_player(
          {
              static_cast<float>(m_maze.get_start().x) + 0.5f,
              0.5f,
              static_cast<float>(m_maze.get_start().y) + 0.5f
          },
          70,
          static_cast<float>(m_window_width) / static_cast<float>(m_window_height)
      ) {
    m_maze.initialize_rendering();
    configure_lighting();
}

Game::~Game() = default;

// called periodically
void Game::fixed_update(double delta) {
}

// called on every frame
void Game::update(double delta) {
    m_player.update(static_cast<float>(delta), m_maze);
    update_player_light();

    Renderer::begin_frame(m_player.get_camera(), m_lighting, m_window_width, m_window_height);

    Renderer::submit(m_maze.get_renderables());
    m_minimap.update(m_player.get_position(), m_window_width, m_window_height);
    Renderer::submit(m_minimap.get_renderables());
    Renderer::end_frame();
}

void Game::configure_lighting() {
    configure_point_light(m_lighting.point_lights[0], m_maze.get_start(), {0.2f, 0.4f, 1.0f});
    configure_point_light(m_lighting.point_lights[1], m_maze.get_exit(), {1.0f, 0.2f, 0.08f});
    m_lighting.point_light_count = 2;
}

void Game::update_player_light() {
    m_lighting.spot_lights[0].position = m_player.get_camera().get_position();
    m_lighting.spot_lights[0].direction = m_player.get_camera().get_front();
}
void Game::configure_point_light(
    Point_Light &light,
    const glm::ivec2 &cell,
    const glm::vec3 &color
) {
    light.position = {
        static_cast<float>(cell.x) + 0.5f,
        0.5f,
        static_cast<float>(cell.y) + 0.5f
    };
    light.ambient = color * 0.08f;
    light.diffuse = color * 1.5f;
    light.specular = color;
    light.constant = 1.0f;
    light.linear = 0.35f;
    light.quadratic = 0.44f;
}

void Game::on_resize(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0) {
        return;
    }

    m_window_width = width;
    m_window_height = height;

    m_player.get_camera().set_aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
}

#include "game.h"

#include "maze_generator.h"
#include "renderer/renderer.h"
#include "platform/time.h"
#include "platform/input.h"
#include "utils/utils.h"

Game::Game(uint32_t window_width, uint32_t window_height)
    : m_window_width(window_width),
      m_window_height(window_height),

      m_maze(Maze_Generator::generate(31, 31)),
      m_maze_mesh(m_maze.to_mesh(1.0f)),
      m_minimap(m_maze.to_texture2D()),
      m_quad_mesh(Mesh::quad()),
      m_quad_shader(
          Utils::read_entire_file("assets/shaders/quad.vert"),
          Utils::read_entire_file("assets/shaders/quad.frag")
      ),
      m_ui_shader(
          Utils::read_entire_file("assets/shaders/ui.vert"),
          Utils::read_entire_file("assets/shaders/ui.frag")
      ),
      m_ui_color_shader(
          Utils::read_entire_file("assets/shaders/ui_color.vert"),
          Utils::read_entire_file("assets/shaders/ui_color.frag")
      ),
      m_player(
          {
              static_cast<float>(m_maze.get_start().x) + 0.5f,
              0.5f,
              static_cast<float>(m_maze.get_start().y) + 0.5f
          },
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
    m_player.update(deltaf, m_maze);

    // ===== 3D =====
    Renderer::begin_scene(m_player.get_camera());

    Renderer::draw(m_maze_mesh, m_quad_shader);

    Renderer::end_scene();
    // ==============

    // ===== UI =====
    Renderer::begin_ui(m_window_width, m_window_height);

    constexpr float MINIMAP_SIZE = 200.0f;
    constexpr float MARGIN = 20.0f;
    constexpr float MARKER_SIZE = 6.0f;

    const glm::vec2 minimap_position{static_cast<float>(m_window_width) - MINIMAP_SIZE - MARGIN, MARGIN};

    Renderer::draw_ui(
        m_quad_mesh,
        m_ui_shader,
        m_minimap,
        minimap_position,
        {MINIMAP_SIZE, MINIMAP_SIZE}
    );

    const glm::vec3 player_position = m_player.get_position();

    const float normalized_x = player_position.x / static_cast<float>(m_maze.get_width());
    const float normalized_y = player_position.z / static_cast<float>(m_maze.get_height());

    const glm::vec2 marker_position{
        minimap_position.x + normalized_x * MINIMAP_SIZE,
        minimap_position.y + normalized_y * MINIMAP_SIZE
    };

    Renderer::draw_ui_quad(
        m_quad_mesh,
        m_ui_color_shader,
        {
            marker_position.x - MARKER_SIZE * 0.5f,
            marker_position.y - MARKER_SIZE * 0.5f
        },
        {MARKER_SIZE, MARKER_SIZE},
        {0.2f, 0.8f, 0.2f, 1.0f}
    );

    Renderer::end_ui();
    // ==============
}

void Game::on_resize(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0) {
        return;
    }

    m_window_width = width;
    m_window_height = height;

    m_player.get_camera().set_aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
}

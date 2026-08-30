#include "game.h"

#include "maze_generator.h"
#include "renderer/renderer.h"
#include "platform/time.h"
#include "platform/input.h"

Game::Game(uint32_t window_width, uint32_t window_height)
    : m_window_width(window_width),
      m_window_height(window_height),
      m_maze(Maze_Generator::generate(m_maze_size, m_maze_size)),
      m_minimap(std::make_unique<Minimap>(m_maze)),
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
    constexpr uint32_t MAZE_SIZE_STEP = 2;
    constexpr uint32_t MIN_MAZE_SIZE = 11;
    constexpr uint32_t MAX_MAZE_SIZE = 101;

    if (Input::is_key_pressed(Key::R)) {
        regenerate_maze(m_maze_size);
    }
    if (Input::is_key_pressed(Key::UP) && m_maze_size < MAX_MAZE_SIZE) {
        regenerate_maze(m_maze_size + MAZE_SIZE_STEP);
    }
    if (Input::is_key_pressed(Key::DOWN) && m_maze_size > MIN_MAZE_SIZE) {
        regenerate_maze(m_maze_size - MAZE_SIZE_STEP);
    }

    if (Input::is_mouse_captured()) {
        m_player.update_look();
        if (!m_exit_sequence_active) {
            m_player.update_movement(static_cast<float>(delta), m_maze);
            if (has_reached_exit()) {
                begin_exit_sequence();
            }
        }
    }
    if (m_exit_sequence_active) {
        update_exit_sequence(static_cast<float>(delta));
    } else {
        m_maze.update_exit_rat(m_player.get_position(), 0.0f);
    }
    update_player_light();

    Renderer::begin_frame(m_player.get_camera(), m_lighting, m_window_width, m_window_height);

    Renderer::submit(m_maze.get_renderables());
    m_minimap->update(m_player.get_position(), m_window_width, m_window_height);
    Renderer::submit(m_minimap->get_renderables());
    Renderer::end_frame();
}

void Game::configure_lighting() {
    m_lighting.global_ambient = {0.33f, 0.33f, 0.33f};
    m_lighting.spot_lights[0] = {};
    configure_point_light(m_lighting.point_lights[0], m_maze.get_start(), {0.2f, 0.4f, 1.0f});
    configure_point_light(m_lighting.point_lights[1], m_maze.get_exit(), {1.0f, 0.2f, 0.08f});
    m_lighting.point_light_count = 2;
}

void Game::update_player_light() {
    m_lighting.spot_lights[0].position = m_player.get_camera().get_position();
    m_lighting.spot_lights[0].direction = m_player.get_camera().get_front();
}

void Game::regenerate_maze(uint32_t size) {
    m_maze_size = size;
    m_maze = Maze_Generator::generate(size, size);
    m_maze.initialize_rendering();
    m_minimap = std::make_unique<Minimap>(m_maze);
    m_exit_sequence_active = false;
    m_exit_sequence_time = 0.0f;

    m_player.set_position({
        static_cast<float>(m_maze.get_start().x) + 0.5f,
        0.5f,
        static_cast<float>(m_maze.get_start().y) + 0.5f
    });
    configure_lighting();
}

void Game::begin_exit_sequence() {
    m_exit_sequence_active = true;
    m_exit_sequence_time = 0.0f;
    m_lighting.global_ambient *= 0.35f;
    for (uint32_t i = 0; i < m_lighting.point_light_count; ++i) {
        m_lighting.point_lights[i].ambient *= 0.35f;
        m_lighting.point_lights[i].diffuse *= 0.35f;
    }
    m_lighting.spot_lights[0].ambient *= 0.35f;
    m_lighting.spot_lights[0].diffuse *= 0.35f;
}

void Game::update_exit_sequence(float delta) {
    m_exit_sequence_time += delta;
    const float jump_height = glm::max(glm::sin(m_exit_sequence_time * glm::two_pi<float>()), 0.0f) * 0.1f;
    m_maze.update_exit_rat(m_player.get_position(), jump_height);
    if (m_exit_sequence_time >= 3.0f) {
        regenerate_maze(m_maze_size);
    }
}

bool Game::has_reached_exit() const {
    const glm::vec3 &position = m_player.get_position();
    const uint32_t cell_x = static_cast<uint32_t>(position.x);
    const uint32_t cell_y = static_cast<uint32_t>(position.z);
    return m_maze.get_tile(cell_x, cell_y) == Maze_Tile::EXIT;
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

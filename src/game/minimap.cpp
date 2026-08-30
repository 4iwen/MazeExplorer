#include "minimap.h"

#include "utils/utils.h"

Minimap::Minimap(const Maze &maze)
    : m_maze(maze),
      m_texture(maze.to_texture2D()),
      m_quad_mesh(Mesh::quad()),
      m_texture_shader(
          Utils::read_entire_file("assets/shaders/ui.vert"),
          Utils::read_entire_file("assets/shaders/ui.frag")
      ),
      m_color_shader(
          Utils::read_entire_file("assets/shaders/ui_color.vert"),
          Utils::read_entire_file("assets/shaders/ui_color.frag")
      ),
      m_texture_material(m_texture_shader, &m_texture),
      m_marker_material(m_color_shader, nullptr, {0.2f, 0.8f, 0.2f, 1.0f}),
      m_renderables{
          Renderable(m_quad_mesh, m_texture_material, Render_Pass::UI),
          Renderable(m_quad_mesh, m_marker_material, Render_Pass::UI, 1)
      } {
}

void Minimap::update(const glm::vec3 &player_position, uint32_t screen_width, uint32_t screen_height) {
    constexpr float SIZE = 200.0f;
    constexpr float MARGIN = 20.0f;
    constexpr float MARKER_SIZE = 6.0f;

    const glm::vec2 position{static_cast<float>(screen_width) - SIZE - MARGIN, MARGIN};
    m_renderables[0].set_ui_rect(position, {SIZE, SIZE});

    const glm::vec2 marker_position{
        position.x + player_position.x / static_cast<float>(m_maze.get_width()) * SIZE,
        position.y + player_position.z / static_cast<float>(m_maze.get_height()) * SIZE
    };
    m_renderables[1].set_ui_rect(
        marker_position - glm::vec2(MARKER_SIZE * 0.5f),
        {MARKER_SIZE, MARKER_SIZE}
    );
}

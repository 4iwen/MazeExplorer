#pragma once

#include <array>
#include <span>

#include <glm/vec3.hpp>

#include "maze.h"
#include "renderer/material.h"
#include "renderer/mesh.h"
#include "renderer/renderable.h"
#include "renderer/shader.h"
#include "renderer/texture2d.h"

class Minimap {
public:
    explicit Minimap(const Maze &maze);

    void update(const glm::vec3 &player_position, uint32_t screen_width, uint32_t screen_height);

    std::span<const Renderable> get_renderables() const { return m_renderables; }

private:
    const Maze &m_maze;
    Texture2D m_texture;
    Mesh m_quad_mesh;
    Shader m_texture_shader;
    Shader m_color_shader;
    Material m_texture_material;
    Material m_marker_material;
    std::array<Renderable, 2> m_renderables;
};

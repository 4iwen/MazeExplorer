#pragma once

#include <cstdint>
#include <vector>

#include "glm/vec2.hpp"
#include "renderer/mesh.h"
#include "renderer/texture2d.h"

enum class Maze_Tile {
    EMPTY,
    WALL,
    START,
    EXIT
};

class Maze {
public:
    Maze(uint32_t width, uint32_t height);

    Maze_Tile get_tile(uint32_t x, uint32_t y) const;

    void set_tile(uint32_t x, uint32_t y, Maze_Tile tile);

    int32_t get_distance(uint32_t x, uint32_t y) const;

    void set_distance(uint32_t x, uint32_t y, int32_t distance);

    uint32_t get_width() const { return m_width; }
    uint32_t get_height() const { return m_height; }

    Texture2D to_texture2D() const;

    Mesh to_mesh(float tile_size) const;

    void set_start(glm::ivec2 start) { m_start = start; }
    void set_exit(glm::ivec2 exit) { m_exit = exit; }
    void set_max_distance(int32_t max_distance) { m_max_distance = max_distance; }

    const glm::ivec2 &get_start() const { return m_start; }

private:
    uint32_t m_width;
    uint32_t m_height;

    std::vector<Maze_Tile> m_tiles;

    std::vector<int32_t> m_distances;

    glm::ivec2 m_start{0};
    glm::ivec2 m_exit{0};

    int32_t m_max_distance = 0;
};

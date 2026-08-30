#pragma once

#include <cstdint>
#include <memory>
#include <span>
#include <vector>

#include "glm/vec2.hpp"
#include "renderer/mesh.h"
#include "renderer/texture2d.h"
#include "renderer/renderable.h"

enum class Maze_Tile {
    EMPTY,
    WALL,
    START,
    EXIT
};

class Maze {
public:
    Maze(uint32_t width, uint32_t height);

    ~Maze();

    Maze(Maze &&other) noexcept;

    Maze &operator=(Maze &&other) noexcept;

    Maze(const Maze &other) = delete;

    Maze &operator=(const Maze &other) = delete;

    Maze_Tile get_tile(uint32_t x, uint32_t y) const;

    bool is_wall(int32_t x, int32_t y) const;

    void set_tile(uint32_t x, uint32_t y, Maze_Tile tile);

    int32_t get_distance(uint32_t x, uint32_t y) const;

    void set_distance(uint32_t x, uint32_t y, int32_t distance);

    uint32_t get_width() const { return m_width; }
    uint32_t get_height() const { return m_height; }

    Texture2D to_texture2D() const;

    Mesh to_floor_mesh(float tile_size) const;

    Mesh to_wall_mesh(float tile_size) const;

    void initialize_rendering();

    std::span<const Renderable> get_renderables() const;

    void update_exit_rat(const glm::vec3 &player_position, float jump_height);

    void set_start(glm::ivec2 start) { m_start = start; }
    void set_exit(glm::ivec2 exit) { m_exit = exit; }
    void set_max_distance(int32_t max_distance) { m_max_distance = max_distance; }

    const glm::ivec2 &get_start() const { return m_start; }
    const glm::ivec2 &get_exit() const { return m_exit; }

private:
    struct Render_Data;

    Mesh create_mesh(float tile_size, bool include_floor, bool include_walls) const;

    uint32_t m_width;
    uint32_t m_height;

    std::vector<Maze_Tile> m_tiles;

    std::vector<int32_t> m_distances;

    glm::ivec2 m_start{0};
    glm::ivec2 m_exit{0};

    int32_t m_max_distance = 0;

    std::unique_ptr<Render_Data> m_render_data;
};

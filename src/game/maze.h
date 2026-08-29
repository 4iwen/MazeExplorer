#pragma once

#include <cstdint>
#include <vector>

enum class Maze_Tile {
    EMPTY,
    WALL,
    START,
    END
};

class Maze {
public:
    Maze(uint32_t width, uint32_t height);

    Maze_Tile get_tile(uint32_t x, uint32_t y) const;

    void set_tile(uint32_t x, uint32_t y, Maze_Tile tile);

    uint32_t get_width() const { return m_width; }
    uint32_t get_height() const { return m_height; }

    bool is_tile_wall(uint32_t x, uint32_t y) const;

    void print() const;

private:
    uint32_t m_width;
    uint32_t m_height;

    std::vector<Maze_Tile> m_tiles;
};
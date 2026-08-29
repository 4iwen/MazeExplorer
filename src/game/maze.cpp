#include "maze.h"

#include <iostream>
#include <stdexcept>

Maze::Maze(uint32_t width, uint32_t height)
    : m_width(width),
      m_height(height),
      m_tiles(width * height, Maze_Tile::WALL) {
}

Maze_Tile Maze::get_tile(uint32_t x, uint32_t y) const {
    if (x >= m_width || y >= m_height) {
        throw std::out_of_range("x >= m_width || y >= m_height");
    }

    return m_tiles[y * m_width + x];
}

void Maze::set_tile(uint32_t x, uint32_t y, Maze_Tile tile) {
    if (x >= m_width || y >= m_height) {
        throw std::out_of_range("Maze::set_tile");
    }

    m_tiles[y * m_width + x] = tile;
}

bool Maze::is_tile_wall(uint32_t x, uint32_t y) const {
    if (x >= m_width || y >= m_height) {
        throw std::out_of_range("Maze::is_tile_wall");
    }

    return get_tile(x, y) == Maze_Tile::WALL;
}

void Maze::print() const {
    for (uint32_t y = 0; y < m_height; ++y) {
        for (uint32_t x = 0; x < m_width; ++x) {
            if (is_tile_wall(x, y)) {
                std::cout << "#";
            } else {
                std::cout << " ";
            }
        }

        std::cout << '\n';
    }
}
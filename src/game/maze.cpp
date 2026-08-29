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

void Maze::print() const {
    for (uint32_t y = 0; y < m_height; ++y) {
        for (uint32_t x = 0; x < m_width; ++x) {
            Maze_Tile tile = get_tile(x, y);

            if (tile == Maze_Tile::WALL) {
                std::cout << "#";
            } else if (tile == Maze_Tile::START) {
                std::cout << "S";
            } else if (tile == Maze_Tile::EXIT) {
                std::cout << "E";
            } else {
                std::cout << " ";
            }
        }

        std::cout << '\n';
    }
}

Texture2D Maze::to_texture2D() const {
    std::vector<uint8_t> data;
    data.resize(m_width * m_height * 4);

    for (uint32_t y = 0; y < m_height; ++y) {
        for (uint32_t x = 0; x < m_width; ++x) {
            const Maze_Tile tile = get_tile(x, y);

            // Flip the Y
            const uint32_t texture_y = m_height - 1 - y;

            const size_t index = (texture_y * m_width + x) * 4;

            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 0;
            uint8_t a = 255;

            switch (tile) {
                case Maze_Tile::WALL:
                    r = 0;
                    g = 0;
                    b = 0;
                    break;

                case Maze_Tile::EMPTY:
                    r = 255;
                    g = 255;
                    b = 255;
                    break;

                case Maze_Tile::START:
                    r = 0;
                    g = 255;
                    b = 0;
                    break;

                case Maze_Tile::EXIT:
                    r = 255;
                    g = 0;
                    b = 0;
                    break;
            }

            data[index + 0] = r;
            data[index + 1] = g;
            data[index + 2] = b;
            data[index + 3] = a;
        }
    }

    return Texture2D::from_data(
        m_width,
        m_height,
        std::move(data)
    );
}

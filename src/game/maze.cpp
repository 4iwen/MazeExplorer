#include "maze.h"

#include <iostream>
#include <stdexcept>

#include <glm/glm.hpp>

#include "renderer/mesh.h"
#include "renderer/vertex_buffer.h"

Maze::Maze(uint32_t width, uint32_t height)
    : m_width(width),
      m_height(height),
      m_tiles(width * height, Maze_Tile::WALL),
      m_distances(width * height, -1) {
}

Maze_Tile Maze::get_tile(uint32_t x, uint32_t y) const {
    if (x >= m_width || y >= m_height) {
        throw std::out_of_range("Maze::get_tile");
    }

    return m_tiles[y * m_width + x];
}

void Maze::set_tile(uint32_t x, uint32_t y, Maze_Tile tile) {
    if (x >= m_width || y >= m_height) {
        throw std::out_of_range("Maze::set_tile");
    }

    m_tiles[y * m_width + x] = tile;
}

int32_t Maze::get_distance(uint32_t x, uint32_t y) const {
    if (x >= m_width || y >= m_height) {
        throw std::out_of_range("Maze::get_distance");
    }

    return m_distances[y * m_width + x];
}

void Maze::set_distance(uint32_t x, uint32_t y, int32_t distance) {
    if (x >= m_width || y >= m_height) {
        throw std::out_of_range("Maze::set_distance");
    }

    m_distances[y * m_width + x] = distance;
}

Texture2D Maze::to_texture2D() const {
    std::vector<uint8_t> data(m_width * m_height * 4);

    for (uint32_t y = 0; y < m_height; ++y) {
        for (uint32_t x = 0; x < m_width; ++x) {
            const Maze_Tile tile = get_tile(x, y);

            const size_t index = (y * m_width + x) * 4;

            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 0;
            uint8_t a = 255;

            switch (tile) {
                case Maze_Tile::WALL:
                    r = 85;
                    g = 85;
                    b = 85;
                    break;

                case Maze_Tile::EMPTY:
                    r = 255;
                    g = 255;
                    b = 255;
                    break;

                case Maze_Tile::START:
                    r = 0;
                    g = 0;
                    b = 255;
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

Mesh Maze::to_mesh(float tile_size) const {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    vertices.reserve(static_cast<size_t>(m_width) * m_height * 4);
    indices.reserve(static_cast<size_t>(m_width) * m_height * 6);

    constexpr glm::vec4 WALL_COLOR{0.33f, 0.33f, 0.33f, 1.0f};
    constexpr glm::vec4 START_COLOR{.1, .1, .9, 1};
    constexpr glm::vec4 EXIT_COLOR{.9, .1, .1, 1};

    for (uint32_t y = 0; y < m_height; ++y) {
        for (uint32_t x = 0; x < m_width; ++x) {
            const Maze_Tile tile = get_tile(x, y);
            glm::vec4 color;
            if (tile == Maze_Tile::WALL) {
                color = WALL_COLOR;
            } else {
                float t = 0.0f;

                if (m_max_distance > 0) {
                    t = static_cast<float>(get_distance(x, y)) / static_cast<float>(m_max_distance);
                }

                t = glm::clamp(t, 0.0f, 1.0f);

                color = glm::mix(START_COLOR, EXIT_COLOR, t);
            }

            const float x0 = static_cast<float>(x) * tile_size;
            const float x1 = x0 + tile_size;
            const float z0 = static_cast<float>(y) * tile_size;
            const float z1 = z0 + tile_size;

            const uint32_t base = static_cast<uint32_t>(vertices.size());

            vertices.push_back({
                .position = {x0, 0.0f, z0},
                .normal = {0.0f, 1.0f, 0.0f},
                .tangent = {1.0f, 0.0f, 0.0f},
                .uv = {0.0f, 0.0f},
                .color = color
            });
            vertices.push_back({
                .position = {x0, 0.0f, z1},
                .normal = {0.0f, 1.0f, 0.0f},
                .tangent = {1.0f, 0.0f, 0.0f},
                .uv = {0.0f, 1.0f},
                .color = color
            });
            vertices.push_back({
                .position = {x1, 0.0f, z1},
                .normal = {0.0f, 1.0f, 0.0f},
                .tangent = {1.0f, 0.0f, 0.0f},
                .uv = {1.0f, 1.0f},
                .color = color
            });
            vertices.push_back({
                .position = {x1, 0.0f, z0},
                .normal = {0.0f, 1.0f, 0.0f},
                .tangent = {1.0f, 0.0f, 0.0f},
                .uv = {1.0f, 0.0f},
                .color = color
            });

            indices.insert(
                indices.end(),
                {
                    base + 0,
                    base + 1,
                    base + 2,
                    base + 2,
                    base + 3,
                    base + 0
                }
            );
        }
    }

    return {vertices, indices};
}

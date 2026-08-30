#include "maze.h"

#include <iostream>
#include <stdexcept>
#include <array>

#include <glm/glm.hpp>

#include "renderer/mesh.h"
#include "renderer/vertex_buffer.h"
#include "utils/utils.h"

struct Maze::Render_Data {
    Shader shader;
    Texture2D floor_texture;
    Texture2D wall_texture;
    Mesh floor_mesh;
    Mesh wall_mesh;
    Material floor_material;
    Material wall_material;
    std::array<Renderable, 2> renderables;

    explicit Render_Data(const Maze &maze)
        : shader(
              Utils::read_entire_file("assets/shaders/quad.vert"),
              Utils::read_entire_file("assets/shaders/quad.frag")
          ),
          floor_texture(Texture2D::from_file("assets/textures/scifi/floor_desert.png")),
          wall_texture(Texture2D::from_file("assets/textures/scifi/wall_desert_2.png")),
          floor_mesh(maze.to_floor_mesh(1.0f)),
          wall_mesh(maze.to_wall_mesh(1.0f)),
          floor_material(shader, &floor_texture),
          wall_material(shader, &wall_texture),
          renderables{
              Renderable(floor_mesh, floor_material, Render_Pass::WORLD),
              Renderable(wall_mesh, wall_material, Render_Pass::WORLD)
          } {
    }
};

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

bool Maze::is_wall(int32_t x, int32_t y) const {
    return x < 0 ||
           y < 0 ||
           x >= static_cast<int32_t>(m_width) ||
           y >= static_cast<int32_t>(m_height) ||
           get_tile(static_cast<uint32_t>(x), static_cast<uint32_t>(y)) == Maze_Tile::WALL;
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

Mesh Maze::to_floor_mesh(float tile_size) const {
    return create_mesh(tile_size, true, false);
}

Maze::~Maze() = default;

Maze::Maze(Maze &&other) noexcept = default;

Maze &Maze::operator=(Maze &&other) noexcept = default;

Mesh Maze::to_wall_mesh(float tile_size) const {
    return create_mesh(tile_size, false, true);
}

void Maze::initialize_rendering() {
    m_render_data = std::make_unique<Render_Data>(*this);
}

std::span<const Renderable> Maze::get_renderables() const {
    return m_render_data->renderables;
}

Mesh Maze::create_mesh(float tile_size, bool include_floor, bool include_walls) const {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    vertices.reserve(static_cast<size_t>(m_width) * m_height * 12);
    indices.reserve(static_cast<size_t>(m_width) * m_height * 18);

    constexpr glm::vec4 WALL_COLOR{0.33f, 0.33f, 0.33f, 1.0f};
    constexpr glm::vec4 START_COLOR{.1, .1, .9, 1};
    constexpr glm::vec4 EXIT_COLOR{.9, .1, .1, 1};
    constexpr float WALL_HEIGHT = 1.0f;

    const auto add_quad = [&vertices, &indices](
        const glm::vec3 &p0,
        const glm::vec3 &p1,
        const glm::vec3 &p2,
        const glm::vec3 &p3,
        const glm::vec3 &normal,
        const glm::vec4 &color
    ) {
        const uint32_t base = static_cast<uint32_t>(vertices.size());

        vertices.push_back({
            .position = p0,
            .normal = normal,
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {0.0f, 0.0f},
            .color = color
        });
        vertices.push_back({
            .position = p1,
            .normal = normal,
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {1.0f, 0.0f},
            .color = color
        });
        vertices.push_back({
            .position = p2,
            .normal = normal,
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {1.0f, 1.0f},
            .color = color
        });
        vertices.push_back({
            .position = p3,
            .normal = normal,
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {0.0f, 1.0f},
            .color = color
        });

        indices.insert(
            indices.end(),
            {
                base + 0, base + 1, base + 2,
                base + 2, base + 3, base + 0
            }
        );
    };

    for (uint32_t y = 0; y < m_height; ++y) {
        for (uint32_t x = 0; x < m_width; ++x) {
            const Maze_Tile tile = get_tile(x, y);
            if (tile != Maze_Tile::WALL) {
                if (!include_floor) {
                    continue;
                }

                float t = 0.0f;

                if (m_max_distance > 0) {
                    t = static_cast<float>(get_distance(x, y)) / static_cast<float>(m_max_distance);
                }

                t = glm::clamp(t, 0.0f, 1.0f);

                const float x0 = static_cast<float>(x) * tile_size;
                const float x1 = x0 + tile_size;
                const float z0 = static_cast<float>(y) * tile_size;
                const float z1 = z0 + tile_size;

                add_quad(
                    {x0, 0.0f, z0}, {x0, 0.0f, z1},
                    {x1, 0.0f, z1}, {x1, 0.0f, z0},
                    {0.0f, 1.0f, 0.0f},
                    glm::mix(START_COLOR, EXIT_COLOR, t)
                );
                continue;
            }

            if (!include_walls) {
                continue;
            }

            const float x0 = static_cast<float>(x) * tile_size;
            const float x1 = x0 + tile_size;
            const float z0 = static_cast<float>(y) * tile_size;
            const float z1 = z0 + tile_size;

            const int32_t grid_x = static_cast<int32_t>(x);
            const int32_t grid_y = static_cast<int32_t>(y);

            // Add only faces visible from maze corridors.
            if (!is_wall(grid_x, grid_y - 1)) {
                add_quad(
                    {x1, 0.0f, z0},
                    {x0, 0.0f, z0},
                    {x0, WALL_HEIGHT, z0},
                    {x1, WALL_HEIGHT, z0},
                    {0.0f, 0.0f, -1.0f},
                    WALL_COLOR
                );
            }
            if (!is_wall(grid_x, grid_y + 1)) {
                add_quad(
                    {x0, 0.0f, z1},
                    {x1, 0.0f, z1},
                    {x1, WALL_HEIGHT, z1},
                    {x0, WALL_HEIGHT, z1},
                    {0.0f, 0.0f, 1.0f},
                    WALL_COLOR
                );
            }
            if (!is_wall(grid_x - 1, grid_y)) {
                add_quad(
                    {x0, 0.0f, z0},
                    {x0, 0.0f, z1},
                    {x0, WALL_HEIGHT, z1},
                    {x0, WALL_HEIGHT, z0},
                    {-1.0f, 0.0f, 0.0f},
                    WALL_COLOR
                );
            }
            if (!is_wall(grid_x + 1, grid_y)) {
                add_quad(
                    {x1, 0.0f, z1},
                    {x1, 0.0f, z0},
                    {x1, WALL_HEIGHT, z0},
                    {x1, WALL_HEIGHT, z1},
                    {1.0f, 0.0f, 0.0f},
                    WALL_COLOR
                );
            }

            add_quad(
                {x0, WALL_HEIGHT, z0},
                {x0, WALL_HEIGHT, z1},
                {x1, WALL_HEIGHT, z1},
                {x1, WALL_HEIGHT, z0},
                {0.0f, 1.0f, 0.0f},
                WALL_COLOR
            );
        }
    }

    return {vertices, indices};
}

#include "maze_generator.h"

#include <algorithm>
#include <array>
#include <random>

#include <glm/vec2.hpp>

// Based on:
// https://weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracking.html

namespace {
    std::mt19937 rng(std::random_device{}());
}

void Maze_Generator::carve_passages_from(
    int32_t x,
    int32_t y,
    Maze &maze,
    glm::ivec2 &farthest,
    int32_t &max_distance
) {
    std::array<glm::ivec2, 4> directions = {
        {
            {0, -2},
            {0, 2},
            {-2, 0},
            {2, 0}
        }
    };

    std::shuffle(directions.begin(), directions.end(), rng);

    const int32_t current_distance = maze.get_distance(x, y);

    for (const glm::ivec2 &direction: directions) {
        int nx = x + direction.x;
        int ny = y + direction.y;

        if (
            nx <= 0 ||
            ny <= 0 ||
            nx >= static_cast<int>(maze.get_width()) - 1 ||
            ny >= static_cast<int>(maze.get_height()) - 1
        ) {
            continue;
        }

        // Still a wall = not visited yet.
        if (maze.get_tile(nx, ny) != Maze_Tile::WALL) {
            continue;
        }

        // Remove the wall between current and next cell.
        int wall_x = (x + nx) / 2;
        int wall_y = (y + ny) / 2;

        maze.set_tile(wall_x, wall_y, Maze_Tile::EMPTY);
        maze.set_distance(wall_x, wall_y, current_distance + 1);
        maze.set_tile(nx, ny, Maze_Tile::EMPTY);
        const int32_t next_distance = current_distance + 2;
        maze.set_distance(nx, ny, next_distance);

        if (next_distance > max_distance) {
            max_distance = next_distance;
            farthest = {nx, ny};
        }

        carve_passages_from(
            nx,
            ny,
            maze,
            farthest,
            max_distance
        );
    }
}

Maze Maze_Generator::generate(uint32_t width, uint32_t height) {
    if (
        width < 5 ||
        height < 5 ||
        width % 2 == 0 ||
        height % 2 == 0
    ) {
        throw std::invalid_argument(
            "Maze dimensions must be odd and at least 5"
        );
    }

    Maze maze(width, height);

    // Number of logical cells along each dimension.
    uint32_t cells_x = (width - 1) / 2;
    uint32_t cells_y = (height - 1) / 2;

    std::uniform_int_distribution<uint32_t> x_dist(0, cells_x - 1);
    std::uniform_int_distribution<uint32_t> y_dist(0, cells_y - 1);

    glm::ivec2 start = {
        static_cast<int32_t>(x_dist(rng) * 2 + 1),
        static_cast<int32_t>(y_dist(rng) * 2 + 1)
    };
    maze.set_tile(start.x, start.y, Maze_Tile::EMPTY);
    maze.set_distance(start.x, start.y, 0);

    glm::ivec2 farthest = start;
    int32_t max_distance = 0;

    carve_passages_from(
        start.x,
        start.y,
        maze,
        farthest,
        max_distance
    );

    maze.set_start(start);
    maze.set_exit(farthest);
    maze.set_max_distance(max_distance);

    maze.set_tile(start.x, start.y, Maze_Tile::START);
    maze.set_tile(farthest.x, farthest.y, Maze_Tile::EXIT);

    return maze;
}

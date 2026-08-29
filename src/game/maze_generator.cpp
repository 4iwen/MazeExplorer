#include "maze_generator.h"

#include <algorithm>
#include <array>
#include <random>

#include <glm/vec2.hpp>

// Based on:
// https://weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracking.html

std::mt19937 rng(std::random_device{}());

void Maze_Generator::carve_passages_from(
    int32_t x,
    int32_t y,
    Maze &maze
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
        maze.set_tile(nx, ny, Maze_Tile::EMPTY);

        carve_passages_from(nx, ny, maze);
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

    glm::ivec2 start = {1, 1};
    maze.set_tile(start.x, start.y, Maze_Tile::EMPTY);

    carve_passages_from(1, 1, maze);

    return maze;
}

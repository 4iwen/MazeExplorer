#pragma once

#include <glm/vec2.hpp>

#include "maze.h"

class Maze_Generator {
public:
    static Maze generate(uint32_t width, uint32_t height);

private:
    static void carve_passages_from(
        int32_t x,
        int32_t y,
        Maze &maze,
        int32_t depth,
        glm::ivec2 &farthest,
        int32_t &max_depth
    );
};

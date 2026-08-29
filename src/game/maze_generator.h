#pragma once

#include "maze.h"

class Maze_Generator {
public:
    static Maze generate(uint32_t width, uint32_t height);
private:
    static void carve_passages_from(int32_t x, int32_t y, Maze&maze);
};
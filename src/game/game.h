#pragma once

#include "maze.h"
#include "minimap.h"
#include "renderer/lighting.h"
#include "renderer/renderable.h"
#include "player.h"

class Game {
public:
    Game(uint32_t window_width, uint32_t window_height);

    ~Game();

    Game(const Game &other) = delete;

    Game &operator=(const Game &other) = delete;

    void fixed_update(double delta);

    void update(double delta);

    void on_resize(uint32_t width, uint32_t height);

private:
    void configure_lighting();

    void update_player_light();

    static void configure_point_light(
        Point_Light &light,
        const glm::ivec2 &cell,
        const glm::vec3 &color
    );

    uint32_t m_window_width;
    uint32_t m_window_height;

    Maze m_maze;
    Minimap m_minimap;

    Player m_player;
    Lighting m_lighting;
};

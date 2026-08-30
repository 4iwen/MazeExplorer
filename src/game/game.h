#pragma once

#include <memory>

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

    void regenerate_maze(uint32_t size);

    bool has_reached_exit() const;

    void begin_exit_sequence();

    void update_exit_sequence(float delta);

    static void configure_point_light(
        Point_Light &light,
        const glm::ivec2 &cell,
        const glm::vec3 &color
    );

    uint32_t m_window_width;
    uint32_t m_window_height;
    uint32_t m_maze_size = 15;
    bool m_exit_sequence_active = false;
    float m_exit_sequence_time = 0.0f;

    Maze m_maze;
    std::unique_ptr<Minimap> m_minimap;

    Player m_player;
    Lighting m_lighting;
};

#pragma once

#include "maze.h"
#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/camera.h"
#include "player.h"
#include "renderer/texture2d.h"

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
    uint32_t m_window_width;
    uint32_t m_window_height;

    Maze m_maze;
    Mesh m_maze_mesh;
    Texture2D m_minimap;

    Mesh m_quad_mesh;

    Shader m_quad_shader;
    Shader m_ui_shader;
    Shader m_ui_color_shader;

    Player m_player;
};

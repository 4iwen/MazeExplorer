#pragma once

#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/camera.h"
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
    uint32_t m_window_width;
    uint32_t m_window_height;

    float m_mouse_sensitivity = 0.003f;
    float m_movement_speed = 1.0f;

    Mesh m_quad_mesh;
    Shader m_quad_shader;
    Player m_player;

    //Camera m_camera;
};

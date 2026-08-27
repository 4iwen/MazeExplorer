#pragma once

#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/camera.h"

class Game {
public:
    Game();

    ~Game();
    Game(const Game &other) = delete;
    Game &operator=(const Game &other) = delete;

    void fixed_update(double delta);

    void update(double delta);

private:
    Mesh m_quad_mesh;
    Shader m_quad_shader;
    Camera m_camera;
};

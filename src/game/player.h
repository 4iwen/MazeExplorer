#pragma once

#include <glm/glm.hpp>

#include "renderer/camera.h"
#include "aabb.h"

class Player {
public:
    Player(
        const glm::vec3 &position,
        float fov,
        float aspect_ratio
    );

    void update(float delta);

    void update_look(float delta);

    void update_movement(float delta);

    Camera get_camera() const { return m_camera; }
    Camera &get_camera() { return m_camera; }

private:
    glm::vec3 m_position = {0, 0, 0};
    float m_move_speed = 0.8f;
    float m_mouse_sensitivity = 0.003f;

    //float m_radius = 0.3f;

    Camera m_camera;
    AABB m_aabb;
};

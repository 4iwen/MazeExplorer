#pragma once

#include <glm/glm.hpp>

#include "renderer/camera.h"
#include "aabb.h"

class Maze;

class Player {
public:
    Player(
        const glm::vec3 &position,
        float fov,
        float aspect_ratio
    );

    void update_look();

    void update_movement(float delta, const Maze &maze);

    const Camera &get_camera() const { return m_camera; }
    Camera &get_camera() { return m_camera; }
    const glm::vec3 &get_position() const { return m_position; }

    void set_position(const glm::vec3 &position);

private:
    bool collides_with_wall(const Maze &maze, const glm::vec3 &position) const;

    glm::vec3 m_position = {0, 0, 0};
    float m_move_speed = 2.5f;
    float m_mouse_sensitivity = 0.003f;
    float m_collision_radius = 0.25f;
    float m_collision_height = 1.8f;

    Camera m_camera;
};

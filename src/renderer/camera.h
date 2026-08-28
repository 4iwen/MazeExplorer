#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// We mean perspective for now, orthographic may come later.
class Camera {
public:
    Camera(
        float fov,
        float aspect_ratio = 1.0f,
        glm::vec3 position = {0, 0, 0},
        glm::vec3 rotation = {0, 0, 0},
        float near = 0.1f,
        float far = 1000.0f
    );

    void set_aspect_ratio(float aspect_ratio) { m_aspect_ratio = aspect_ratio; }

    void set_position(glm::vec3 position) { m_position = position; update_vectors(); }
    glm::vec3 get_position() const { return m_position; }

    void set_rotation(glm::vec3 rotation) { m_rotation = rotation; update_vectors(); }
    glm::vec3 get_rotation() const { return m_rotation; }

    glm::mat4 get_projection_matrix() const;

    glm::mat4 get_view_matrix() const;

    glm::vec3 get_front() const { return m_front; }
    glm::vec3 get_up() const { return m_up; }
    glm::vec3 get_right() const { return m_right; }

    void update_vectors();

private:
    float m_fov_radians;
    float m_aspect_ratio;
    glm::vec3 m_position;
    glm::vec3 m_rotation; // Pitch, yaw, roll
    float m_near_plane;
    float m_far_plane;

    glm::vec3 m_front = {0, 0, -1};
    glm::vec3 m_right = {1, 0, 0};
    glm::vec3 m_up = {0, 1, 0};
    glm::vec3 m_world_up = {0, 1, 0};
};

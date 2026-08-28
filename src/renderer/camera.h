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

    void set_position(glm::vec3 position) { m_position = position; }

    void set_rotation(glm::vec3 rotation) { m_orientation = rotation; }

    glm::mat4 get_projection_matrix() const;

    glm::mat4 get_view_matrix() const;

private:
    float m_fov_radians;
    float m_aspect_ratio;
    float m_near_plane;
    float m_far_plane;
    glm::vec3 m_position;
    glm::quat m_orientation;
};

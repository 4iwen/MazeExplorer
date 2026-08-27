#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


// We mean perspective for now, orthographic may come later.
class Camera {
public:
    Camera(
        float fov,
        float aspect_ratio,
        glm::vec3 position = {0, 0, 0},
        glm::vec3 rotation = {0, 0, 0},
        float near = 0.1f,
        float far = 1000.0f
    );

private:
    float m_fov_radians;
    float m_aspect_ratio;
    float m_near_plane;
    float m_far_plane;
    glm::vec3 m_position;
    glm::quat m_orientation;
};
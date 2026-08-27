#include "camera.h"

Camera::Camera(
    float fov,
    float aspect_ratio,
    glm::vec3 position,
    glm::vec3 rotation,
    float near,
    float far
) : m_fov_radians(glm::radians(fov)),
    m_aspect_ratio(aspect_ratio),
    m_near_plane(near),
    m_far_plane(far),
    m_position(position),
    m_orientation(rotation) {
}

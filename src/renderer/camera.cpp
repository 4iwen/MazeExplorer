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

glm::mat4 Camera::get_projection_matrix() const {
    return glm::perspective(
        m_fov_radians,
        m_aspect_ratio,
        m_near_plane,
        m_far_plane
    );
}

glm::mat4 Camera::get_view_matrix() const {
    glm::mat4 rotation = glm::mat4_cast(glm::conjugate(m_orientation));
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_position);
    return rotation * translation;
}

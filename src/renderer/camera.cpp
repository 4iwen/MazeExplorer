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
    m_position(position),
    m_rotation(rotation),
    m_near_plane(near),
    m_far_plane(far) {
    update_vectors();
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
    glm::vec3 center = m_position + m_front;
    return glm::lookAt(m_position, center, m_up);
}

void Camera::update_vectors() {
    float pitch = m_rotation.x;
    float yaw = m_rotation.y;

    // Calculate front vector.
    glm::vec3 front;
    front.x = glm::cos(pitch) * glm::cos(yaw);
    front.y = glm::sin(pitch);
    front.z = glm::sin(yaw) * glm::cos(pitch);
    m_front = glm::normalize(front);

    // Calculate right and up vectors.
    m_right = glm::normalize(cross(m_front, m_world_up));
    m_up = glm::normalize(cross(m_right, m_front));
}
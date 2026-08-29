#pragma once

#include <glm/vec3.hpp>

class AABB {
public:
    AABB() = default;

    AABB(
        const glm::vec3 &min,
        const glm::vec3 &max
    ) : m_min(min), m_max(max) {
    }

    static AABB from_center_size(
        const glm::vec3 &center,
        const glm::vec3 &size
    ) {
        glm::vec3 half_size = size * 0.5f;
        return AABB(center - half_size, center + half_size);
    }

    const glm::vec3 &get_min() const { return m_min; }
    const glm::vec3 &get_max() const { return m_max; }
    glm::vec3 get_center() const { return (m_min + m_max) * 0.5f; }
    glm::vec3 get_size() const { return m_max - m_min; }

    bool intersects(const AABB &other) const {
        return m_min.x < other.m_max.x &&
               m_max.x > other.m_min.x &&
               m_min.y < other.m_max.y &&
               m_max.y > other.m_min.y &&
               m_min.z < other.m_max.z &&
               m_max.z > other.m_min.z;
    }

    bool intersects_xz(const AABB &other) const {
        return m_min.x < other.m_max.x &&
               m_max.x > other.m_min.x &&
               m_min.z < other.m_max.z &&
               m_max.z > other.m_min.z;
    }

private:
    glm::vec3 m_min;
    glm::vec3 m_max;
};

#include "player.h"

#include <glm/glm.hpp>

#include "maze.h"
#include "platform/input.h"

Player::Player(
    const glm::vec3 &position,
    float fov,
    float aspect_ratio
)
    : m_position(position),
      m_camera({
          fov,
          aspect_ratio,
          position,
          {0, glm::radians(-90.0f), 0}
      }) {
}

void Player::update(float delta, const Maze &maze) {
    if (Input::is_mouse_captured()) {
        update_look();
        update_movement(delta, maze);
    }
}

void Player::update_look() {
    double dx = Input::get_mouse_dx();
    double dy = Input::get_mouse_dy();

    glm::vec3 rotation = m_camera.get_rotation();

    rotation.y += static_cast<float>(dx) * m_mouse_sensitivity;

    rotation.x = glm::clamp(
        rotation.x - static_cast<float>(dy) * m_mouse_sensitivity,
        glm::radians(-89.0f),
        glm::radians(89.0f)
    );

    m_camera.set_rotation(rotation);
}

void Player::update_movement(float delta, const Maze &maze) {
    glm::vec3 movement{0.0f};

    glm::vec3 front = m_camera.get_front();
    glm::vec3 right = m_camera.get_right();

    front.y = 0.0f;
    right.y = 0.0f;

    front = glm::normalize(front);
    right = glm::normalize(right);

    if (Input::is_key_down(Key::W)) {
        movement += front;
    }
    if (Input::is_key_down(Key::S)) {
        movement -= front;
    }
    if (Input::is_key_down(Key::D)) {
        movement += right;
    }
    if (Input::is_key_down(Key::A)) {
        movement -= right;
    }

    if (glm::dot(movement, movement) > 0.0f) {
        movement = glm::normalize(movement);
    }

    glm::vec3 delta_vec = movement * m_move_speed * delta;

    glm::vec3 new_position = m_position;

    new_position.x += delta_vec.x;
    if (!collides_with_wall(maze, new_position)) {
        m_position.x = new_position.x;
    }

    new_position = m_position;
    new_position.z += delta_vec.z;
    if (!collides_with_wall(maze, new_position)) {
        m_position.z = new_position.z;
    }

    m_camera.set_position(m_position);
}

bool Player::collides_with_wall(const Maze &maze, const glm::vec3 &position) const {
    const AABB player_bounds = AABB::from_center_size(
        {position.x, m_collision_height * 0.5f, position.z},
        {m_collision_radius * 2.0f, m_collision_height, m_collision_radius * 2.0f}
    );

    const int32_t min_x = static_cast<int32_t>(glm::floor(position.x - m_collision_radius));
    const int32_t max_x = static_cast<int32_t>(glm::floor(position.x + m_collision_radius));
    const int32_t min_z = static_cast<int32_t>(glm::floor(position.z - m_collision_radius));
    const int32_t max_z = static_cast<int32_t>(glm::floor(position.z + m_collision_radius));

    for (int32_t z = min_z; z <= max_z; ++z) {
        for (int32_t x = min_x; x <= max_x; ++x) {
            if (!maze.is_wall(x, z)) {
                continue;
            }

            const AABB wall_bounds(
                {static_cast<float>(x), 0.0f, static_cast<float>(z)},
                {static_cast<float>(x + 1), m_collision_height, static_cast<float>(z + 1)}
            );

            if (player_bounds.intersects_xz(wall_bounds)) {
                return true;
            }
        }
    }

    return false;
}

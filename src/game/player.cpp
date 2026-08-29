#include "player.h"

#include <glm/glm.hpp>

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

void Player::update(float delta) {
    if (Input::is_mouse_captured()) {
        update_look(delta);
        update_movement(delta);
    }
}

void Player::update_look(float delta) {
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

void Player::update_movement(float delta) {
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

    // Resolve each axis separately.
    glm::vec3 new_position = m_position;

    new_position.x += delta_vec.x;

    new_position.x += delta_vec.x;
    m_position.x = new_position.x;

    new_position = m_position;
    new_position.z += delta_vec.z;
    m_position.z = new_position.z;

    m_camera.set_position(m_position);
}

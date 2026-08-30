#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "material.h"
#include "mesh.h"
#include "render_command.h"

class Renderable final {
public:
    Renderable(
        const Mesh &mesh,
        const Material &material,
        Render_Pass pass,
        uint32_t layer = 0,
        const glm::mat4 &transform = glm::mat4(1.0f)
    ) : m_mesh(mesh),
        m_material(material),
        m_pass(pass), m_layer(layer),
        m_transform(transform) {
    }

    const Mesh &get_mesh() const { return m_mesh; }
    const Material &get_material() const { return m_material; }
    const glm::mat4 &get_transform() const { return m_transform; }
    Render_Pass get_pass() const { return m_pass; }
    uint32_t get_layer() const { return m_layer; }
    void set_transform(glm::mat4 transform) { m_transform = transform; }

    void set_ui_rect(const glm::vec2 &position, const glm::vec2 &size) {
        glm::mat4 transform{1.0f};
        transform = glm::translate(transform, glm::vec3(position + size * 0.5f, 0.0f));
        m_transform = glm::scale(transform, glm::vec3(size, 1.0f));
    }

private:
    const Mesh &m_mesh;
    const Material &m_material;
    Render_Pass m_pass;
    uint32_t m_layer;
    glm::mat4 m_transform;
};

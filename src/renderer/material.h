#pragma once

#include "texture2d.h"
#include "shader.h"
#include <glm/vec4.hpp>

class Material {
public:
    Material(
        const Shader &shader,
        const Texture2D *albedo = nullptr,
        glm::vec4 color = glm::vec4(1.0f)
    ) : m_shader(shader), m_albedo(albedo), m_color(color) {
    }

    void bind(uint32_t texture_slot = 0) const {
        m_shader.use();
        if (m_albedo) {
            m_albedo->bind(texture_slot);
        }
    }

    bool has_albedo() const { return m_albedo != nullptr; }
    const glm::vec4 &get_color() const { return m_color; }
    const void *get_sort_key() const { return &m_shader; }

    void set_mat4(const std::string &name, const glm::mat4 &value) const { m_shader.set_mat4(name, value); }
    void set_int(const std::string &name, int value) const { m_shader.set_int(name, value); }
    void set_float(const std::string &name, float value) const { m_shader.set_float(name, value); }
    void set_vec3(const std::string &name, const glm::vec3 &value) const { m_shader.set_vec3(name, value); }
    void set_vec4(const std::string &name, const glm::vec4 &value) const { m_shader.set_vec4(name, value); }

private:
    const Shader &m_shader;
    const Texture2D *m_albedo;
    glm::vec4 m_color;
};

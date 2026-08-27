#pragma once

#include <cstdint>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void render();

private:
    uint32_t m_vertex_shader;
    uint32_t m_fragment_shader;
    uint32_t m_shader_program;

    uint32_t m_vbo;
    uint32_t m_ebo;
    uint32_t m_vao;
};

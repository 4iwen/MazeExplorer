#pragma once

#include <span>

#include <glm/glm.hpp>

#include "renderer_backend.h"
#include "buffer.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 uv;
};

class Vertex_Buffer : public Buffer {
public:
    explicit Vertex_Buffer(std::span<const Vertex> vertices);

    ~Vertex_Buffer() override;

    Vertex_Buffer(const Vertex_Buffer &other) = delete;

    Vertex_Buffer &operator=(const Vertex_Buffer &other) = delete;

    void bind() const override;

private:
#ifdef RENDERER_OPENGL
    GLuint m_id;
#endif
};

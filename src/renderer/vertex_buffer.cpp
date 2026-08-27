#include "vertex_buffer.h"

#ifdef RENDERER_OPENGL
Vertex_Buffer::Vertex_Buffer(std::span<const Vertex> vertices) : Buffer() {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size_bytes(),
        vertices.data(),
        GL_STATIC_DRAW
    );
}

Vertex_Buffer::~Vertex_Buffer() {
    glDeleteBuffers(1, &m_id);
}

void Vertex_Buffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}
#endif

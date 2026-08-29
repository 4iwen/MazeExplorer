#include "vertex_array.h"

#ifdef RENDERER_OPENGL
Vertex_Array::Vertex_Array() {
    glGenVertexArrays(1, &m_id);
}

Vertex_Array::~Vertex_Array() {
    glDeleteVertexArrays(1, &m_id);
}

void Vertex_Array::bind() const {
    glBindVertexArray(m_id);
}

void Vertex_Array::unbind() const {
    glBindVertexArray(0);
}

void Vertex_Array::add_vertex_buffer(const Vertex_Buffer &vertex_buffer) const {
    bind();

    vertex_buffer.bind();

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position))
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, normal))
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, tangent))
    );
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(
        3, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, uv))
    );
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(
        4, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, color))
    );
    glEnableVertexAttribArray(4);

    unbind();
}

void Vertex_Array::add_index_buffer(const Index_Buffer &index_buffer) const {
    bind();

    index_buffer.bind();

    unbind();
}
#endif

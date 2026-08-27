#pragma once

#include "renderer_backend.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

class Vertex_Array {
public:
    Vertex_Array();

    ~Vertex_Array();

    Vertex_Array(const Vertex_Array &other) = delete;

    Vertex_Array &operator=(const Vertex_Array &other) = delete;

    void bind() const;

    void unbind() const;

    void add_vertex_buffer(const Vertex_Buffer &vertex_buffer) const;

    void add_index_buffer(const Index_Buffer &index_buffer) const;

private:
#ifdef RENDERER_OPENGL
    GLuint m_id;
#endif
};

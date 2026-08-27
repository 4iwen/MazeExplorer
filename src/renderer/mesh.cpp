#include "mesh.h"

Mesh::Mesh(
    const std::vector<Vertex> &vertices,
    const std::vector<uint32_t> &indices
) : m_vertices(vertices),
    m_indices(indices),
    m_vertex_buffer(vertices),
    m_index_buffer(indices) {
    m_vertex_array.add_vertex_buffer(m_vertex_buffer);
    m_vertex_array.add_index_buffer(m_index_buffer);
}

void Mesh::draw() const {
    m_vertex_array.bind();

#ifdef RENDERER_OPENGL
    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(m_indices.size()),
        GL_UNSIGNED_INT,
        nullptr
    );
#endif

    m_vertex_array.unbind();
}

Mesh Mesh::quad() {
    const std::vector<Vertex> vertices = {
        {
            .position = {-0.5f, 0.5f, 0.0f},
            .normal = {0.0f, 0.0f, 1.0f},
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {0.0f, 1.0f}
        },
        {
            .position = {0.5f, 0.5f, 0.0f},
            .normal = {0.0f, 0.0f, 1.0f},
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {1.0f, 1.0f}
        },
        {
            .position = {0.5f, -0.5f, 0.0f},
            .normal = {0.0f, 0.0f, 1.0f},
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {1.0f, 0.0f}
        },
        {
            .position = {-0.5f, -0.5f, 0.0f},
            .normal = {0.0f, 0.0f, 1.0f},
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {0.0f, 0.0f}
        }
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 3, 0
    };

    return {vertices, indices};
}

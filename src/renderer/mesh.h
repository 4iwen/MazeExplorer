#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

class Mesh {
public:
    Mesh(
        const std::vector<Vertex> &vertices,
        const std::vector<uint32_t> &indices
    );

    void draw() const;

    static Mesh quad();

private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    Vertex_Array m_vertex_array;
    Vertex_Buffer m_vertex_buffer;
    Index_Buffer m_index_buffer;
};

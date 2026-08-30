#pragma once

#include <glm/glm.hpp>

#include <string>
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

    glm::vec3 get_min_position() const;

    static Mesh quad();

    static Mesh from_obj(const std::string &file_path);

private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    Vertex_Array m_vertex_array;
    Vertex_Buffer m_vertex_buffer;
    Index_Buffer m_index_buffer;
};

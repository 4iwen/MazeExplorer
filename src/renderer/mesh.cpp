#include "mesh.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

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
            .uv = {0.0f, 1.0f},
            .color = {1.0f, 1.0f, 1.0f, 1.0f}
        },
        {
            .position = {0.5f, 0.5f, 0.0f},
            .normal = {0.0f, 0.0f, 1.0f},
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {1.0f, 1.0f},
            .color = {1.0f, 1.0f, 1.0f, 1.0f}
        },
        {
            .position = {0.5f, -0.5f, 0.0f},
            .normal = {0.0f, 0.0f, 1.0f},
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {1.0f, 0.0f},
            .color = {1.0f, 1.0f, 1.0f, 1.0f}
        },
        {
            .position = {-0.5f, -0.5f, 0.0f},
            .normal = {0.0f, 0.0f, 1.0f},
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = {0.0f, 0.0f},
            .color = {1.0f, 1.0f, 1.0f, 1.0f}
        }
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 3, 0
    };

    return {vertices, indices};
}

glm::vec3 Mesh::get_min_position() const {
    glm::vec3 minimum = m_vertices.front().position;
    for (const Vertex &vertex : m_vertices) {
        minimum = glm::min(minimum, vertex.position);
    }
    return minimum;
}

Mesh Mesh::from_obj(const std::string &file_path) {
    // This intentionally supports the OBJ subset used by the project assets: v, vt, vn, and f.
    std::ifstream file(file_path);
    if (!file) {
        throw std::runtime_error("Failed to open OBJ file: " + file_path);
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<std::string, uint32_t> vertex_indices;

    const auto get_vertex_index = [&positions, &uvs, &normals, &vertices, &vertex_indices](const std::string &token) {
        const auto existing = vertex_indices.find(token);
        if (existing != vertex_indices.end()) {
            return existing->second;
        }

        std::istringstream stream(token);
        std::string part;
        std::getline(stream, part, '/');
        const uint32_t position_index = static_cast<uint32_t>(std::stoul(part) - 1);
        std::getline(stream, part, '/');
        const uint32_t uv_index = static_cast<uint32_t>(std::stoul(part) - 1);
        std::getline(stream, part, '/');
        const uint32_t normal_index = static_cast<uint32_t>(std::stoul(part) - 1);

        const uint32_t index = static_cast<uint32_t>(vertices.size());
        vertices.push_back({
            .position = positions.at(position_index),
            .normal = normals.at(normal_index),
            .tangent = {1.0f, 0.0f, 0.0f},
            .uv = uvs.at(uv_index),
            .color = {1.0f, 1.0f, 1.0f, 1.0f}
        });
        vertex_indices.emplace(token, index);
        return index;
    };

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string type;
        stream >> type;

        if (type == "v") {
            glm::vec3 position;
            stream >> position.x >> position.y >> position.z;
            positions.push_back(position);
        } else if (type == "vt") {
            glm::vec2 uv;
            stream >> uv.x >> uv.y;
            uvs.push_back(uv);
        } else if (type == "vn") {
            glm::vec3 normal;
            stream >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (type == "f") {
            std::vector<uint32_t> face;
            std::string token;
            while (stream >> token) {
                face.push_back(get_vertex_index(token));
            }

            for (size_t i = 2; i < face.size(); ++i) {
                indices.insert(indices.end(), {face[0], face[i - 1], face[i]});
            }
        }
    }

    return {vertices, indices};
}

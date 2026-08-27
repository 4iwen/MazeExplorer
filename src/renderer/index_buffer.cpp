#include "index_buffer.h"

#include <vector>
#include <cstdint>

#ifdef RENDERER_OPENGL
Index_Buffer::Index_Buffer(std::span<const uint32_t> indices) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size_bytes(),
        indices.data(),
        GL_STATIC_DRAW
    );
}

Index_Buffer::~Index_Buffer() {
    glDeleteBuffers(1, &m_id);
}

void Index_Buffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}
#endif

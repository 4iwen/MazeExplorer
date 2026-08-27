#pragma once

#include <span>

#include "renderer_backend.h"
#include "buffer.h"

class Index_Buffer : public Buffer {
public:
    explicit Index_Buffer(std::span<const uint32_t> indices);

    ~Index_Buffer() override;

    Index_Buffer(const Index_Buffer &other) = delete;

    Index_Buffer &operator=(const Index_Buffer &other) = delete;

    void bind() const override;

private:
#ifdef RENDERER_OPENGL
    GLuint m_id;
#endif
};

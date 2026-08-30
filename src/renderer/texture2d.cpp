#include "texture2d.h"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D Texture2D::from_file(const std::string &file_path) {
    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);

    stbi_uc *pixels = stbi_load(file_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels) {
        throw std::runtime_error("Failed to load texture: " + file_path);
    }

    std::vector<uint8_t> data(
        pixels,
        pixels + static_cast<size_t>(width) * height * STBI_rgb_alpha
    );
    stbi_image_free(pixels);

    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height), std::move(data)};
}

Texture2D Texture2D::from_data(
    uint32_t width,
    uint32_t height,
    std::vector<uint8_t> data
) {
    return {width, height, std::move(data)};
}

#ifdef RENDERER_OPENGL
void Texture2D::bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

Texture2D::Texture2D(
    uint32_t width,
    uint32_t height,
    std::vector<uint8_t> data
) : m_id(0),
    m_width(width),
    m_height(height) {
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // Filtering and wrapping hardcoded for now
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_NEAREST
    );
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_NEAREST
    );
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE
    );
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE
    );

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data.data()
    );

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_id);
}
#endif

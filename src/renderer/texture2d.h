#pragma once

#include <string>
#include <vector>

#include "renderer_backend.h"

class Texture2D {
public:
    ~Texture2D();

    Texture2D(const Texture2D &other) = delete;

    Texture2D &operator=(const Texture2D &other) = delete;

    static Texture2D from_file(const std::string &file_path);

    static Texture2D from_data(
        uint32_t width,
        uint32_t height,
        std::vector<uint8_t> data
    );

    void bind(uint32_t slot) const;

private:
    Texture2D(
        uint32_t width,
        uint32_t height,
        std::vector<uint8_t> data
    );

    uint32_t m_id;
    uint32_t m_width;
    uint32_t m_height;
};

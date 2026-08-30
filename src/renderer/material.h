#pragma once

#include "texture2d.h"

class Material {
public:
    explicit Material(const Texture2D &albedo) : m_albedo(albedo) {
    }

    void bind(uint32_t texture_slot = 0) const {
        m_albedo.bind(texture_slot);
    }

private:
    const Texture2D &m_albedo;
};

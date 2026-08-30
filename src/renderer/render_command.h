#pragma once

#include <glm/mat4x4.hpp>
#include <cstdint>

#include "material.h"
#include "mesh.h"

enum class Render_Pass {
    WORLD,
    UI
};

struct Render_Command {
    const Mesh *mesh;
    const Material *material;
    glm::mat4 transform;
    Render_Pass pass;
    uint32_t layer;
};

#pragma once

#include <array>
#include <cstdint>

#include <glm/vec3.hpp>

struct Point_Light {
    glm::vec3 position{0.0f};
    glm::vec3 ambient{0.0f};
    glm::vec3 diffuse{1.0f};
    glm::vec3 specular{1.0f};
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};

struct Spot_Light {
    glm::vec3 position{0.0f};
    glm::vec3 direction{0.0f, 0.0f, -1.0f};
    glm::vec3 ambient{0.0f};
    glm::vec3 diffuse{1.0f, 0.95f, 0.85f};
    glm::vec3 specular{1.0f, 0.95f, 0.85f};
    float cutoff = 0.940f;
    float outer_cutoff = 0.820f;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};

// Scene-wide lighting values shared by all 3D materials.
struct Lighting {
    static constexpr uint32_t MAX_POINT_LIGHTS = 2;
    static constexpr uint32_t MAX_SPOT_LIGHTS = 1;

    glm::vec3 global_ambient{0.33f};
    std::array<Point_Light, MAX_POINT_LIGHTS> point_lights{};
    std::array<Spot_Light, MAX_SPOT_LIGHTS> spot_lights{};
    uint32_t point_light_count = 0;
    uint32_t spot_light_count = 1;
};

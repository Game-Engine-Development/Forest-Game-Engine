#pragma once

#include <glm/vec3.hpp>

using Filename = std::string;
using RefCount = std::size_t;

enum struct ImageType {
    RGBA_IMG = 0,
    RGB_IMG = 1,
};

struct Transform {
    glm::vec3 pos{}, rotation{}, scale{};
};

#pragma once

#include <array>
#include <iostream>

#include <glm/glm.hpp>

struct Plane {
    std::array<float, 4> equation{};
    glm::vec3 origin;
    glm::vec3 normal{};
    std::array<glm::vec3, 3> points{};

    Plane(glm::vec3 origin, glm::vec3 normal);
    Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    [[nodiscard]] bool isFrontFacingTo(glm::vec3 direction) const;
    [[nodiscard]] double signedDistanceTo(glm::vec3 point) const;
};

#pragma once

#include <array>
#include <glm/glm.hpp>
#include <iostream>

class Plane {
public:
    std::array<float, 4> equation;
    glm::vec3 origin;
    glm::vec3 normal;
    std::array<glm::vec3, 3> points;

    Plane(const glm::vec3& origin, const glm::vec3& normal);
    Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float flipped);
    [[nodiscard]] bool isFrontFacingTo(const glm::vec3& direction) const;
    [[nodiscard]] double signedDistanceTo(const glm::vec3& point) const;
};

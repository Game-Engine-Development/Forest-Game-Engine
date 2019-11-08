#pragma once

#include <glm/glm.hpp>
#include <iostream>

class Plane {
public:
    float equation[4];
    glm::vec3 origin;
    glm::vec3 normal;
    Plane(const glm::vec3& origin, const glm::vec3& normal);
    Plane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
    [[nodiscard]] bool isFrontFacingTo(const glm::vec3& direction) const;
    [[nodiscard]] double signedDistanceTo(const glm::vec3& point) const;
};

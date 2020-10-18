#pragma once

#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>

using Coordinate = glm::vec3;
using LengthVec3 = glm::vec3;

struct BoundingBox {
    Coordinate center{};
    LengthVec3 halfWidths{};

    [[nodiscard]] constexpr bool containsPoint(Coordinate point) const noexcept;

    [[nodiscard]] constexpr bool intersectsAABB(const BoundingBox &other) const noexcept;
};



struct BoundingSphere {
    Coordinate center{};
    double radius{};

    [[nodiscard]] constexpr bool isPointInsideSphere(Coordinate point) const noexcept;

    [[nodiscard]] bool intersectsOtherSphere(BoundingSphere other) const noexcept;

    [[nodiscard]] bool intersectWithBox(BoundingBox box) const noexcept;

};
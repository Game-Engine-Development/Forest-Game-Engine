#pragma once

#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>

#include "Headers/Engine/Utils/MiscUtils.h"

using Coordinate = glm::vec3;
using LengthVec3 = glm::vec3;

struct BoundingBox {
    Coordinate center{};
    LengthVec3 halfWidths{};

    [[nodiscard]] bool containsPoint(Coordinate point) const noexcept;

    [[nodiscard]] bool intersectsAABB(const BoundingBox &other) const noexcept;

    [[nodiscard]] glm::vec3 get_vmin() const noexcept;
    [[nodiscard]] glm::vec3 get_vmax() const noexcept;
};



struct BoundingSphere {
    Coordinate center{};
    double radius{};

    [[nodiscard]] constexpr bool isPointInsideSphere(Coordinate point) const noexcept;

    [[nodiscard]] bool intersectsOtherSphere(BoundingSphere other) const noexcept;

    [[nodiscard]] bool intersectWithBox(BoundingBox box) const noexcept;
};
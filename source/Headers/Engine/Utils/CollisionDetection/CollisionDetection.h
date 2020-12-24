#pragma once

#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>

#include "Headers/Engine/Utils/MiscUtils.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"



struct Projection {
    glm::vec2 points; //x = min, y = max
    [[nodiscard]] bool overlap(Projection other) const;
};


struct ConvexShape {
    std::vector<Vertex> vertices;

    //axis == normal
    [[nodiscard]] Projection project(glm::vec3 axis) const;
};

//uses SAT (separating axis theorem)
[[nodiscard]] bool convexShapeIntersection(const ConvexShape &convexShape1, const ConvexShape &convexShape2);





struct BoundingBox {
    Coordinate center{};
    LengthVec3 halfWidths{};

    [[nodiscard]] bool containsPoint(Coordinate point) const noexcept;

    [[nodiscard]] bool intersectsAABB(const BoundingBox &other) const noexcept;

    [[nodiscard]] bool intersectsRay(Ray r) const noexcept;

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
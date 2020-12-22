#include "Headers/Engine/Utils/CollisionDetection/CollisionDetection.h"


[[nodiscard]] bool BoundingBox::containsPoint(const Coordinate point) const noexcept {
    const bool containsX = mathRound(point.x, 5) < mathRound((center.x + halfWidths[0]), 5) && mathRound(point.x, 5) >= mathRound((center.x - halfWidths[0]), 5);
    const bool containsY = mathRound(point.y, 5) < mathRound((center.y + halfWidths[1]), 5) && mathRound(point.y, 5) >= mathRound((center.y - halfWidths[1]), 5);
    const bool containsZ = mathRound(point.z, 5) < mathRound((center.z + halfWidths[2]), 5) && mathRound(point.z, 5) >= mathRound((center.z - halfWidths[2]), 5);

    return containsX && containsY && containsZ;
}

[[nodiscard]] bool BoundingBox::intersectsAABB(const BoundingBox &other) const noexcept {
    const bool edgeX = (mathRound((other.center.x - other.halfWidths[0]), 5) <= mathRound((center.x + halfWidths[0]), 5) && mathRound((other.center.x - other.halfWidths[0]), 5) >= mathRound((center.x - halfWidths[0]), 5))
                       || (mathRound((other.center.x + other.halfWidths[0]), 5) >= mathRound((center.x - halfWidths[0]), 5) && mathRound((other.center.x + other.halfWidths[0]), 5) <= mathRound((center.x + halfWidths[0]), 5));

    const bool edgeY = (mathRound((other.center.y - other.halfWidths[1]), 5) <= mathRound((center.y + halfWidths[1]), 5) && mathRound((other.center.y - other.halfWidths[1]), 5) >= mathRound((center.y - halfWidths[1]), 5))
                       || (mathRound((other.center.y + other.halfWidths[1]), 5) >= mathRound((center.y - halfWidths[1]), 5) && mathRound((other.center.y + other.halfWidths[1]), 5) <= mathRound((center.y + halfWidths[1]), 5));

    const bool edgeZ = (mathRound((other.center.z - other.halfWidths[2]), 5) <= mathRound((center.z + halfWidths[2]), 5) && mathRound((other.center.z - other.halfWidths[2]), 5) >= mathRound((center.z - halfWidths[2]), 5))
                       || (mathRound((other.center.z + other.halfWidths[2]), 5) >= mathRound((center.z - halfWidths[2]), 5) && mathRound((other.center.z + other.halfWidths[2]), 5) <= mathRound((center.z + halfWidths[2]), 5));


    const bool smallerX = (mathRound((other.center.x + other.halfWidths[0]), 5) >= mathRound((center.x + halfWidths[0]), 5) && mathRound((other.center.x - other.halfWidths[0]), 5) <= mathRound((center.x - halfWidths[0]), 5));
    const bool smallerY = (mathRound((other.center.y + other.halfWidths[1]), 5) >= mathRound((center.y + halfWidths[1]), 5) && mathRound((other.center.y - other.halfWidths[1]), 5) <= mathRound((center.y - halfWidths[1]), 5));
    const bool smallerZ = (mathRound((other.center.z + other.halfWidths[2]), 5) >= mathRound((center.z + halfWidths[2]), 5) && mathRound((other.center.z - other.halfWidths[2]), 5) <= mathRound((center.z - halfWidths[2]), 5));


    return ((edgeX || smallerX) && (edgeY || smallerY) && (edgeZ || smallerZ));
}

[[nodiscard]] bool BoundingBox::intersectsRay(const Ray r) const noexcept {
    const glm::vec3 dir_inv = 1.0f/r.slope;

    double t1 = (get_vmin()[0] - r.origin[0])*dir_inv[0];
    double t2 = (get_vmax()[0] - r.origin[0])*dir_inv[0];

    double tmin = std::min(t1, t2);
    double tmax = std::max(t1, t2);

    for (int i = 1; i < 3; ++i) {
        t1 = (get_vmin()[i] - r.origin[i])*dir_inv[i];
        t2 = (get_vmax()[i] - r.origin[i])*dir_inv[i];

        tmin = std::max(tmin, std::min(std::min(t1, t2), tmax));
        tmax = std::min(tmax, std::max(std::max(t1, t2), tmin));
    }

    return tmax > std::max(tmin, 0.0);
}

[[nodiscard]] glm::vec3 BoundingBox::get_vmin() const noexcept {
    return center - halfWidths;
}
[[nodiscard]] glm::vec3 BoundingBox::get_vmax() const noexcept {
    return center + halfWidths;
}













[[nodiscard]] constexpr bool BoundingSphere::isPointInsideSphere(const Coordinate point) const noexcept {
    // we are using multiplications because is faster than calling std::pow
    const double distanceSquared = (point.x - center.x) * (point.x - center.x) +
                                   (point.y - center.y) * (point.y - center.y) +
                                   (point.z - center.z) * (point.z - center.z);
    //sqrt is pricey, we instead compare the squared results
    return distanceSquared < (radius*radius);
}

[[nodiscard]] bool BoundingSphere::intersectsOtherSphere(const BoundingSphere other) const noexcept {
    // we are using multiplications because it's faster than calling std::pow
    const double distance = std::sqrt((center.x - other.center.x) * (center.x - other.center.x) +
                                     (center.y - other.center.y) * (center.y - other.center.y) +
                                     (center.z - other.center.z) * (center.z - other.center.z));
    return distance < (radius + other.radius);
}

[[nodiscard]] bool BoundingSphere::intersectWithBox(const BoundingBox box) const noexcept {
    // get box closest point to sphere center by clamping
    const double x = std::max((box.center.x - box.halfWidths.x), std::min(center.x, (box.center.x + box.halfWidths.x)));
    const double y = std::max((box.center.y - box.halfWidths.y), std::min(center.y, (box.center.y + box.halfWidths.y)));
    const double z = std::max((box.center.z - box.halfWidths.z), std::min(center.z, (box.center.z + box.halfWidths.z)));

    // this is the same as isPointInsideSphere
    const double distance = std::sqrt((x - center.x) * (x - center.x) +
                                     (y - center.y) * (y - center.y) +
                                     (z - center.z) * (z - center.z));

    return distance < radius;
}

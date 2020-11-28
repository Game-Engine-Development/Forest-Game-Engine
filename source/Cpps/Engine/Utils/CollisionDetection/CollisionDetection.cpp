#include "Headers/Engine/Utils/CollisionDetection/CollisionDetection.h"


[[nodiscard]] bool BoundingBox::containsPoint(const Coordinate point) const noexcept {
    const bool containsX = mathRound(point.x, 5) < mathRound((center.x + halfWidths[0]), 5) && mathRound(point.x, 5) >= mathRound((center.x - halfWidths[0]), 5);
    const bool containsY = mathRound(point.y, 5) < mathRound((center.y + halfWidths[1]), 5) && mathRound(point.y, 5) >= mathRound((center.y - halfWidths[1]), 5);
    const bool containsZ = mathRound(point.z, 5) < mathRound((center.z + halfWidths[2]), 5) && mathRound(point.z, 5) >= mathRound((center.z - halfWidths[2]), 5);

    return containsX && containsY && containsZ;
}

[[nodiscard]] bool BoundingBox::intersectsAABB(const BoundingBox &other) const noexcept {
    const bool edgeX = ((other.center.x - other.halfWidths[0]) <= (center.x + halfWidths[0]) && (other.center.x - other.halfWidths[0]) >= (center.x - halfWidths[0]))
                       || ((other.center.x + other.halfWidths[0]) >= (center.x - halfWidths[0]) && (other.center.x + other.halfWidths[0]) <= (center.x + halfWidths[0]));

    const bool edgeY = ((other.center.y - other.halfWidths[1]) <= (center.y + halfWidths[1]) && (other.center.y - other.halfWidths[1]) >= (center.y - halfWidths[1]))
                       || ((other.center.y + other.halfWidths[1]) >= (center.y - halfWidths[1]) && (other.center.y + other.halfWidths[1]) <= (center.y + halfWidths[1]));

    const bool edgeZ = ((other.center.z - other.halfWidths[2]) <= (center.z + halfWidths[2]) && (other.center.z - other.halfWidths[2]) >= (center.z - halfWidths[2]))
                       || ((other.center.z + other.halfWidths[2]) >= (center.z - halfWidths[2]) && (other.center.z + other.halfWidths[2]) <= (center.z + halfWidths[2]));


    const bool smallerX = ((other.center.x + other.halfWidths[0]) >= (center.x + halfWidths[0]) && (other.center.x - other.halfWidths[0]) <= (center.x - halfWidths[0]));
    const bool smallerY = ((other.center.y + other.halfWidths[1]) >= (center.y + halfWidths[1]) && (other.center.y - other.halfWidths[1]) <= (center.y - halfWidths[1]));
    const bool smallerZ = ((other.center.z + other.halfWidths[2]) >= (center.z + halfWidths[2]) && (other.center.z - other.halfWidths[2]) <= (center.z - halfWidths[2]));


    return ((edgeX || smallerX) && (edgeY || smallerY) && (edgeZ || smallerZ));
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

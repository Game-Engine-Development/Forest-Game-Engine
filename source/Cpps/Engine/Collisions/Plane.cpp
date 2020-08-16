#include "Headers/Engine/Collisions/Plane.h"

Plane::Plane(const glm::vec3 origin, const glm::vec3 normal)
: normal(normal), origin(origin),
equation{normal.x, normal.y, normal.z,
         -(normal.x*origin.x+normal.y*origin.y+normal.z*origin.z)}
{}

// Construct from triangle:
Plane::Plane(const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3)
: points{p1, p2, p3}, origin(p1)
{
    normal = glm::cross((p2-p1), (p3-p1));
    normal = -glm::normalize(normal);

    equation[0] = normal.x;
    equation[1] = normal.y;
    equation[2] = normal.z;
    equation[3] = -(normal.x*origin.x+normal.y*origin.y+normal.z*origin.z);
}

bool Plane::isFrontFacingTo(const glm::vec3 direction) const {
    const double dot = glm::dot(normal, direction);
    return (dot <= 0);
}

double Plane::signedDistanceTo(const glm::vec3 point) const {
    return (glm::dot(point, normal)) + equation[3];
}
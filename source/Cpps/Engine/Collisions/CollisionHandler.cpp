#include "Headers/Engine/Collisions/CollisionHandler.h"

typedef unsigned int uint32;
#define in(a) ((uint32&) a)
bool checkPointInTriangle(
        const glm::vec3 point,
        const std::array<glm::vec3, 3> &points)
{
    const glm::vec3 e10 = points[1] - points[0];
    const glm::vec3 e20 = points[2] - points[0];

    const float a = glm::dot(e10, e10);
    const float b = glm::dot(e10, e20);
    const float c = glm::dot(e20, e20);
    const float ac_bb = (a * c) - (b * b);

    const glm::vec3 vp = point - points[0];

    const float d = glm::dot(vp, e10);
    const float e = glm::dot(vp, e20);
    const float x = (d * c) - (e * b);
    const float y = (e * a) - (d * b);
    const float z = x + y - ac_bb;

    return ((in(z)& ~(in(x)|in(y))) & 0x80000000); //@todo bitwise magic should have a comment
}

std::pair<bool, float> getLowestRoot(const float a, const float b, const float c,
                                     const float maxR, const float root)
{
    const float determinant = b * b - 4.0f * a * c;

    if (determinant < 0.0f) return {false, root};

    const float sqrtD = std::sqrt(determinant);

    float r1 = (-b - sqrtD) / (2 * a);
    float r2 = (-b + sqrtD) / (2 * a);
    if (r1 > r2) {
        std::swap(r1, r2);
    }

    if (r1 > 0 && r1 < maxR) {
        return {true, r1};
    }
    if (r2 > 0 && r2 < maxR) {
        return {true, r2};
    }

// DNE
    return {false, root};
}

float squareAndSum(const glm::vec3 data) {
    return data.x * data.x + data.y * data.y + data.z * data.z;
}



CollisionHandler::CollisionHandler(Entity *entity)
: m_entity(entity)
{
    move.eRadius = m_entity->getScale();
}

void CollisionHandler::calculateCollisions(const std::vector<Plane> &planes) {
    std::for_each(planes.begin(), planes.end(), [&](const Plane &plane) {checkTriangle(plane);});
}

void CollisionHandler::checkTriangle(const Plane &trianglePlane) {
    if (!trianglePlane.isFrontFacingTo(move.eSpaceMovementNormalized)) {
        float t0 = 0.0f, t1 = 1.0f;

        const double signedDistToTrianglePlane = trianglePlane.signedDistanceTo(move.eSpaceStartingPos);
        const float normalDotVelocity = glm::dot(trianglePlane.normal, move.eSpaceMovement);

        const bool zeroNormalDotVelocity = (normalDotVelocity == 0.0f);
        const bool largeDistToTriPlane = std::abs(signedDistToTrianglePlane) >= 1.0f;

        const bool embeddedInPlane = zeroNormalDotVelocity && !largeDistToTriPlane;

// check if parallel / already inside of plane
        if (zeroNormalDotVelocity && largeDistToTriPlane) {
            return;
        }

        if(!zeroNormalDotVelocity) {
            t0 = (-1.0 - signedDistToTrianglePlane) / normalDotVelocity;
            t1 = (1.0 - signedDistToTrianglePlane) / normalDotVelocity;
// Swap so t0 < t1
            if (t0 > t1) {
                std::swap(t0, t1);
            }
            if (t0 > 1.0f || t1 < 0.0f) {
                return;
            }

            t0 = std::clamp(t0, 0.0f, 1.0f);
            t1 = std::clamp(t1, 0.0f, 1.0f);
        }



        glm::vec3 collisionPoint{};
        bool foundCollison = false;
        float t = 1.0;
        if (!embeddedInPlane) {
            const glm::vec3 planeIntersectionPoint{
                    move.eSpaceStartingPos - trianglePlane.normal + t0 * move.eSpaceMovement
            };
            if (checkPointInTriangle(planeIntersectionPoint, trianglePlane.points)) {
                foundCollison = true;
                t = t0;
                collisionPoint = planeIntersectionPoint;
            }
        }



        if (!foundCollison) {
            const glm::vec3 velocity = move.eSpaceMovement;
            const glm::vec3 base = move.eSpaceStartingPos;
            const float velocitySquaredLength = squareAndSum(velocity);

            float a = velocitySquaredLength, b, c;
            float newT{};

            for(int i = 0; i < 3; ++i) {
                const glm::vec3 cTemp = trianglePlane.points[i] - base;

                b = 2.0f * (glm::dot(base - trianglePlane.points[i], velocity));
                c = squareAndSum(cTemp) - 1.0f;
                const auto[foundRoot, root] = getLowestRoot(a, b, c, t, newT);
                newT = root;
                if (foundRoot) {
                    t = newT;
                    foundCollison = true;
                    collisionPoint = trianglePlane.points[i];
                }
            }


// Check against edges:
            for(int i = 0; i < 3; ++i) {
                const glm::vec3 edge = trianglePlane.points[(i+1)%3] - trianglePlane.points[i];
                const glm::vec3 baseToVertex = trianglePlane.points[i] - base;
                const float edgeSquaredLength = squareAndSum(edge);
                const float edgeDotVelocity = glm::dot(velocity, edge);
                const float edgeDotBaseToVertex = glm::dot(baseToVertex, edge);
// Calculate parameters for equation
                a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
                b = static_cast<float>(
                        edgeSquaredLength
                        * (2 * glm::dot(baseToVertex, velocity))
                        - 2.0 * edgeDotVelocity * edgeDotBaseToVertex
                );
                c = edgeSquaredLength *
                        (1 - squareAndSum(baseToVertex))
                        + edgeDotBaseToVertex * edgeDotBaseToVertex;

                const auto[foundRoot, root] = getLowestRoot(a, b, c, t, newT);
                newT = root;
                if (foundRoot) {
// Check if intersection is within line segment:
                    float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
                              edgeSquaredLength;
                    if (f >= 0.0 && f <= 1.0) {
// intersection took place within segment.
                        t = newT;
                        foundCollison = true;
                        collisionPoint = trianglePlane.points[i] + f * edge;
                    }
                }
            }
        }



        if (foundCollison) {
// distance to collision: ’t’ is time of collision
            const float distToCollision = t * glm::vec3::length();
            if (!move.foundCollision || distToCollision < move.nearestDistance) {
                move.nearestDistance = distToCollision;
                move.intersectionPoint = collisionPoint;
                move.foundCollision = true;
            }
        }
    }
}

std::vector<Plane> CollisionHandler::calculateCollidablePlanes(std::vector<Plane> &planes) {
    std::vector<Plane> nearbyPlanes;
    for(const Plane& plane : planes) {
        for(const glm::vec3& point : plane.points) {
            const auto withinRange = [](const Entity &entity, const glm::vec3 point) -> bool {
                bool inRange = true;
                for(int i = 0; i < glm::vec3::length(); ++i) {
                    inRange &= std::abs(point[i] - entity.getPos()[i]) < 10;
                }
                return inRange;
            };

            if(withinRange(*m_entity, point)) {
                nearbyPlanes.push_back(plane);
                break;
            }
        }
    }
    return nearbyPlanes;
}

void CollisionHandler::collideAndSlide(
        const glm::vec3 vel,
        const glm::vec3 gravity,
        std::vector<Entity> &entities)
{
    m_hitEntity = nullptr;
    move.startingPos = m_entity->getPos();
    move.movement = vel;
// change values into elipseSpace
    glm::vec3 eSpacePosition = move.startingPos / move.eRadius;
    glm::vec3 eSpaceVelocity = move.movement / move.eRadius;
    collisionRecursionDepth = 0;
    glm::vec3 finalPosition = collideWithWorld(eSpacePosition, eSpaceVelocity, entities);
// repeat for gravity
    float startingPos = finalPosition.y;
    move.startingPos = finalPosition * move.eRadius;
    move.movement = gravity;
    eSpaceVelocity = gravity / move.eRadius;
    collisionRecursionDepth = 0;
    finalPosition = collideWithWorld(finalPosition, eSpaceVelocity, entities);
    if(finalPosition.y <= startingPos - gravity.y && move.foundCollision) {
        currentGravity.y = 0;
        inAir = false;
    } else if(finalPosition.y >= startingPos - gravity.y && move.foundCollision) {
        currentGravity.y = 0;
    }

    finalPosition *= move.eRadius;
    assert(m_entity != nullptr);
    m_entity->setPos(finalPosition);
}

glm::vec3 CollisionHandler::collideWithWorld(
        const glm::vec3 pos,
        const glm::vec3 vel,
        std::vector<Entity> &entities)
{
    float unitScale = unitsPerMeter / 100.0f;
    float veryCloseDistance = 0.005f * unitScale;
    if (collisionRecursionDepth > 5) {
        return pos;
    }
    move.eSpaceMovement = vel;
    move.eSpaceMovementNormalized = glm::normalize(vel);
    move.eSpaceStartingPos = pos;
    move.foundCollision = false;
// Check for collisions

    std::for_each(entities.begin(), entities.end(), [&](const Entity &entity) {calculateCollisions(entity.getPlanes());});
    if (!move.foundCollision) {
        return pos + vel;
    }
    glm::vec3 destinationPoint = pos + vel;
    glm::vec3 newBasePoint = pos;
    if (move.nearestDistance >= veryCloseDistance) {
        glm::vec3 V = vel;
        V = glm::normalize(V) * (float)(move.nearestDistance - veryCloseDistance);
        newBasePoint = move.eSpaceStartingPos + V;
        V = glm::normalize(V);
        move.intersectionPoint -= veryCloseDistance * V;
    }
    glm::vec3 slidePlaneOrigin = move.intersectionPoint;
    glm::vec3 slidePlaneNormal = newBasePoint-move.intersectionPoint;
    slidePlaneNormal = glm::normalize(slidePlaneNormal);
    Plane slidingPlane(slidePlaneOrigin, slidePlaneNormal);
    glm::vec3 newDestinationPoint = destinationPoint
            - static_cast<float>(slidingPlane.signedDistanceTo(destinationPoint))
            * slidePlaneNormal;
    glm::vec3 newVelocityVector = newDestinationPoint - move.intersectionPoint;

    if (std::sqrt(
            std::pow(newVelocityVector.x, 2)
            + std::pow(newVelocityVector.y, 2)
            + std::pow(newVelocityVector.z, 2)) < veryCloseDistance)
    {
        return newBasePoint;
    }

    ++collisionRecursionDepth;
    return collideWithWorld(newBasePoint, newVelocityVector, entities);
}

void CollisionHandler::calculateTerrainCollisions(glm::vec3 &finalMove) {
    glm::vec3 newMove;
    bool cantGetOver = false;
    float height;
    double currentHeight = m_entity->getPos().y - m_entity->getVerticalOffset();
    float dist = std::sqrt(finalMove.x*finalMove.x + finalMove.z*finalMove.z);
    for(int i = 0; i < static_cast<int>(dist * 10); ++i) {
        newMove = glm::normalize(finalMove) * (float)i;
        height = m_entity->getScale().y + currentTerrain->getTerrainHeight(
                m_entity->getPos().x + newMove.x,
                m_entity->getPos().z + newMove.z
        );
        if(height >= m_entity->getPos().y + simGravity(static_cast<float>(i)) - 1) {
            cantGetOver = true;
            break;
        }
    }
    for(int i = 0; i < static_cast<int>(dist); ++i) {
        newMove = glm::normalize(finalMove) * (float)i;
        height = m_entity->getScale().y + currentTerrain->getTerrainHeight(
                m_entity->getPos().x + newMove.x,
                m_entity->getPos().z + newMove.z
        );
        if(height > currentHeight + m_entity->getScale().y && cantGetOver) {
            finalMove = glm::normalize(finalMove) * (float)(i - 1);
            break;
        } else {
            currentHeight = height;
        }
    }
}

void CollisionHandler::updateGravity() {
    if((m_entity->getPos().y <=
        currentTerrain->getTerrainHeight(m_entity->getPos().x, m_entity->getPos().z) + 1)
        && (!move.foundCollision))
    {
        m_entity->setPosY(currentTerrain->getTerrainHeight(m_entity->getPos().x,
                                                                    m_entity->getPos().z) + 1);
        currentGravity.y = 0;
        inAir = false;
    } else {
        currentGravity.y += CollisionHandler::GRAVITY.y;
    }
}

float CollisionHandler::simGravity(const float tics) const {
    return currentGravity.y + (CollisionHandler::GRAVITY.y * tics);
}

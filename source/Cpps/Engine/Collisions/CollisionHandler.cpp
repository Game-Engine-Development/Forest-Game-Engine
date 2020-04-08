#include "Headers/Engine/Collisions/CollisionHandler.h"

CollisionHandler::CollisionHandler() = default;

CollisionHandler::CollisionHandler(Entity *entity) {
    this->m_entity = entity;
    move.eRadius = m_entity->getScale();
}

void CollisionHandler::calculateCollisions(std::vector<Plane> &planes, Entity* entity) {
    std::vector nearbyPlanes = planes; //calculateCollidablePlanes(planes);
    for(const Plane& plane : nearbyPlanes) {
        checkTriangle(plane, entity);
    }
}

void CollisionHandler::checkTriangle(const Plane &trianglePlane, Entity* entity) {
    if (!trianglePlane.isFrontFacingTo(move.eSpaceMovementNormalized)) {
        double t0, t1;
        bool embeddedInPlane = false;
        double signedDistToTrianglePlane = trianglePlane.signedDistanceTo(move.eSpaceStartingPos);
        float normalDotVelocity = glm::dot(trianglePlane.normal, move.eSpaceMovement);
// check if parrallel / already inside of plane
        if (normalDotVelocity == 0.0f) {
            if (fabs(signedDistToTrianglePlane) >= 1.0f) {
                return;
            } else {
                embeddedInPlane = true;
                t0 = 0.0;
                t1 = 1.0;
            }
        } else {
            t0 = (-1.0 - signedDistToTrianglePlane) / normalDotVelocity;
            t1 = (1.0 - signedDistToTrianglePlane) / normalDotVelocity;
// Swap so t0 < t1
            if (t0 > t1) {
                double temp = t1;
                t1 = t0;
                t0 = temp;
            }
            if (t0 > 1.0f || t1 < 0.0f) {
                return;
            }
            if (t0 < 0.0) t0 = 0.0;
            if (t1 < 0.0) t1 = 0.0;
            if (t0 > 1.0) t0 = 1.0;
            if (t1 > 1.0) t1 = 1.0;
        }
        glm::vec3 collisionPoint;
        bool foundCollison = false;
        float t = 1.0;
        if (!embeddedInPlane) {
            glm::vec3 planeIntersectionPoint = (
                    move.eSpaceStartingPos - trianglePlane.normal
                    + static_cast<float>(t0) *
                    move.eSpaceMovement
            );
            if (checkPointInTriangle(
                    planeIntersectionPoint,
                    trianglePlane.points[0],
                    trianglePlane.points[1],
                    trianglePlane.points[2]
                )) {
                foundCollison = true;
                t = static_cast<float>(t0);
                collisionPoint = planeIntersectionPoint;
            }
        }
        if (!foundCollison) {
            glm::vec3 velocity = move.eSpaceMovement;
            glm::vec3 base = move.eSpaceStartingPos;
            float velocitySquaredLength = velocity.x * velocity.x
                    + velocity.y * velocity.y
                    + velocity.z * velocity.z;
            float a,b,c;
            float newT;
            a = velocitySquaredLength;
// P1
            b = 2.0f*(glm::dot(base-trianglePlane.points[0], velocity));
            glm::vec3 cTemp = trianglePlane.points[0] - base;
            c = static_cast<float>(
                    cTemp.x * cTemp.x
                    + cTemp.y * cTemp.y
                    + cTemp.z * cTemp.z - 1.0
            );
            if (getLowestRoot(a,b,c, t, &newT)) {
                t = newT;
                foundCollison = true;
                collisionPoint = trianglePlane.points[0];
            }
// P2
            b = 2.0f*(glm::dot(base-trianglePlane.points[1], velocity));
            cTemp = trianglePlane.points[1] - base;
            c = static_cast<float>(
                    cTemp.x * cTemp.x
                    + cTemp.y * cTemp.y
                    + cTemp.z * cTemp.z
                    - 1.0
            );
            if (getLowestRoot(a,b,c, t, &newT)) {
                t = newT;
                foundCollison = true;
                collisionPoint = trianglePlane.points[1];
            }
// P3
            b = 2.0f*(glm::dot(base-trianglePlane.points[2], velocity));
            cTemp = trianglePlane.points[2] - base;
            c = static_cast<float>(
                    cTemp.x * cTemp.x
                    + cTemp.y * cTemp.y
                    + cTemp.z * cTemp.z
                    - 1.0
            );
            if (getLowestRoot(a,b,c, t, &newT)) {
                t = newT;
                foundCollison = true;
                collisionPoint = trianglePlane.points[2];
            }
// Check agains edges:
// p1 -> p2:
            glm::vec3 edge = trianglePlane.points[1]-trianglePlane.points[0];
            glm::vec3 baseToVertex = trianglePlane.points[0] - base;
            float edgeSquaredLength = edge.x * edge.x + edge.y * edge.y + edge.z * edge.z;
            float edgeDotVelocity = glm::dot(velocity, edge);
            float edgeDotBaseToVertex = glm::dot(baseToVertex, edge);
// Calculate parameters for equation
            a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
            b = static_cast<float>(
                    edgeSquaredLength
                    * (2 * glm::dot(baseToVertex, velocity))
                    - 2.0 * edgeDotVelocity*edgeDotBaseToVertex
            );
            c = edgeSquaredLength * (
                    1 - (
                            baseToVertex.x * baseToVertex.x
                            + baseToVertex.y * baseToVertex.y
                            + baseToVertex.z * baseToVertex.z
                         )
            ) + edgeDotBaseToVertex*edgeDotBaseToVertex;

// Does the swept sphere collide against infinite edge?
            if (getLowestRoot(a,b,c, t, &newT)) {
// Check if intersection is within line segment:
                float f=(edgeDotVelocity*newT-edgeDotBaseToVertex)/
                        edgeSquaredLength;
                if (f >= 0.0 && f <= 1.0) {
// intersection took place within segment.
                    t = newT;
                    foundCollison = true;
                    collisionPoint = trianglePlane.points[0] + f*edge;
                }
            }
// p2 -> p3:
            edge = trianglePlane.points[2]-trianglePlane.points[1];
            baseToVertex = trianglePlane.points[1] - base;
            edgeSquaredLength = edge.x * edge.x + edge.y * edge.y + edge.z * edge.z;
            edgeDotVelocity = glm::dot(velocity, edge);
            edgeDotBaseToVertex = glm::dot(baseToVertex, edge);
            a = edgeSquaredLength
                    * -velocitySquaredLength
                    + edgeDotVelocity*edgeDotVelocity;
            b = static_cast<float>(edgeSquaredLength
                    * (2 * glm::dot(baseToVertex, velocity))
                    -2.0 * edgeDotVelocity * edgeDotBaseToVertex
            );
            c = edgeSquaredLength * (
                    1 - (
                            baseToVertex.x * baseToVertex.x
                            + baseToVertex.y * baseToVertex.y +
                            baseToVertex.z * baseToVertex.z
                         )
            ) + edgeDotBaseToVertex*edgeDotBaseToVertex;

            if (getLowestRoot(a, b, c, t, &newT)) {
                float f=(edgeDotVelocity*newT-edgeDotBaseToVertex)/
                        edgeSquaredLength;
                if (f >= 0.0 && f <= 1.0) {
                    t = newT;
                    foundCollison = true;
                    collisionPoint = trianglePlane.points[1] + f*edge;
                }
            }
// p3 -> p1:
            edge = trianglePlane.points[0]-trianglePlane.points[2];
            baseToVertex = trianglePlane.points[2] - base;
            edgeSquaredLength = edge.x * edge.x + edge.y * edge.y + edge.z * edge.z;
            edgeDotVelocity = glm::dot(velocity, edge);
            edgeDotBaseToVertex = glm::dot(baseToVertex, edge);
            a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
            b = static_cast<float>(edgeSquaredLength*(2*glm::dot(baseToVertex, velocity))
                    - 2.0*edgeDotVelocity*edgeDotBaseToVertex
            );

            c = edgeSquaredLength * (
                    1 - (
                            baseToVertex.x * baseToVertex.x
                            + baseToVertex.y * baseToVertex.y
                            + baseToVertex.z * baseToVertex.z
                         )
            ) + edgeDotBaseToVertex*edgeDotBaseToVertex;

            if (getLowestRoot(a,b,c, t, &newT)) {
                float f=(edgeDotVelocity*newT-edgeDotBaseToVertex)/
                        edgeSquaredLength;
                if (f >= 0.0 && f <= 1.0) {
                    t = newT;
                    foundCollison = true;
                    collisionPoint = trianglePlane.points[2] + f*edge;
                }
            }
        }
// Set result:



//shady
        if (foundCollison) {
// distance to collision: ’t’ is time of collision
            float distToCollision = t * glm::vec3::length();
// Does this triangle qualify for the closest hit?
// it does if it’s the first hit or the closest
            if (!move.foundCollision || distToCollision < move.nearestDistance) {
                if(entity->checkIfAnimal() && m_entity->checkIfBullet()) {
                    m_hitEntity = entity;
                } else {
                    m_hitEntity = nullptr;
                }
// Collision information nessesary for sliding
                move.hitPlayer = entity->checkIfPlayerEntity();
                move.nearestDistance = distToCollision;
                move.intersectionPoint = collisionPoint;
                move.foundCollision = true;
            }
        }
    }
}

std::vector<Plane> CollisionHandler::calculateCollidablePlanes(
        std::vector<Plane> &planes
    ) {
    std::vector<Plane> nearbyPlanes;
    for(const Plane& plane : planes) {
        for(const glm::vec3& point : plane.points) {
            if(
                    point.x < m_entity->getPos().x + 10
                    && point.x > m_entity->getPos().x - 10
                    && point.y < m_entity->getPos().y + 10
                    && point.y > m_entity->getPos().y - 10
                    && point.z < m_entity->getPos().z + 10
                    && point.z > m_entity->getPos().z - 10
                ) {
                nearbyPlanes.push_back(plane);
                break;
            }
        }
    }
    return nearbyPlanes;
}

typedef unsigned int uint32;
#define in(a) ((uint32&) a)
bool CollisionHandler::checkPointInTriangle(
        const glm::vec3& point,
        const glm::vec3& pa,
        const glm::vec3& pb,
        const glm::vec3& pc
    ) {
    glm::vec3 e10=pb-pa;
    glm::vec3 e20=pc-pa;
    float a = glm::dot(e10, e10);
    float b = glm::dot(e10, e20);
    float c = glm::dot(e20, e20);
    float ac_bb=(a*c)-(b*b);
    glm::vec3 vp(point.x-pa.x, point.y-pa.y, point.z-pa.z);
    float d = glm::dot(vp, e10);
    float e = glm::dot(vp, e20);
    float x = (d*c)-(e*b);
    float y = (e*a)-(d*b);
    float z = x+y-ac_bb;
    return (( in(z)& ~(in(x)|in(y)) ) & 0x80000000);
}

void CollisionHandler::collideAndSlide(
        const glm::vec3& vel,
        const glm::vec3& gravity,
        std::vector<Entity*> &entities
    ) {
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
    if(m_hitEntity != nullptr) {
        m_hitEntity->hit = true;
    }
    finalPosition *= move.eRadius;
    assert(m_entity != nullptr);
    m_entity->setPos(finalPosition);
}

glm::vec3 CollisionHandler::collideWithWorld(
        const glm::vec3& pos,
        const glm::vec3& vel,
        std::vector<Entity*> &entities
    ) {
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
    for(Entity *entity : entities) {
        if(entity != m_entity  && !(entity->checkIfPlayerEntity() && m_entity->checkIfBullet())) {
            calculateCollisions(entity->planes, entity);
        }
    }
    if (!move.foundCollision) {
        return pos + vel;
    }
    hitPlayer = move.hitPlayer;
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
            - static_cast<float>(slidingPlane.signedDistanceTo(destinationPoint)
    ) * slidePlaneNormal;
    glm::vec3 newVelocityVector = newDestinationPoint - move.intersectionPoint;

    if (std::sqrt(
            std::pow(newVelocityVector.x, 2)
            + std::pow(newVelocityVector.y, 2)
            + std::pow(newVelocityVector.z, 2)
        ) < veryCloseDistance || m_entity->checkIfBullet()) {
        return newBasePoint;
    }

    ++collisionRecursionDepth;
    return collideWithWorld(newBasePoint, newVelocityVector, entities);
}

bool CollisionHandler::getLowestRoot(float a, float b, float c, float maxR, float* root) {
    float determinant = b * b - 4.0f * a * c;
    if (determinant < 0.0f) return false;
    float sqrtD = std::sqrt(determinant);
    float r1 = (-b - sqrtD) / (2 * a);
    float r2 = (-b + sqrtD) / (2 * a);
    if (r1 > r2) {
        float temp = r2;
        r2 = r1;
        r1 = temp;
    }
    if (r1 > 0 && r1 < maxR) {
        *root = r1;
        return true;
    }
    if (r2 > 0 && r2 < maxR) {
        *root = r2;
        return true;
    }
// DNE
    return false;
}

void CollisionHandler::calculateTerrainCollisions(glm::vec3 &finalMove) {
    glm::vec3 newMove;
    bool cantGetOver = false;
    float height;
    float currentHeight = m_entity->getPos().y;
    float dist = std::sqrt(finalMove.x*finalMove.x + finalMove.z*finalMove.z);
    for(unsigned int i = 0; i < static_cast<int>(dist * 10); ++i) {
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
    for(unsigned int i = 0; i < static_cast<int>(dist); ++i) {
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
    if(m_entity->getPos().y <= currentTerrain->getTerrainHeight(
            m_entity->getPos().x,
            m_entity->getPos().z) + 1
        ) {
        glm::vec3 newPos(
                m_entity->getPos().x,
                currentTerrain->getTerrainHeight(
                        m_entity->getPos().x,
                        m_entity->getPos().z) + 1,
                        m_entity->getPos().z
                );
        m_entity->setPos(newPos);
        currentGravity.y = 0;
        inAir = false;
    } else {
        currentGravity.y += CollisionHandler::GRAVITY.y;
    }
}

float CollisionHandler::simGravity(float tics) {
    return currentGravity.y + (CollisionHandler::GRAVITY.y * tics);
}

#include "Headers/Game/Player.h"

Player::Player() = default;

Player::Player(Camera *camera1, Entity *container1) {
    camera = camera1;
    playerEntity = container1;
    move.eRadius = playerEntity->getScale();
}

void Player::setHeight(){
    if(inAir) {
        float terrainHeight = 1 + currentTerrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z);
        if(playerEntity->getPos().y + jumpingSpeed <= terrainHeight) {
            inAir = false;
            jumpingSpeed = 0;
            glm::vec3 newPos(playerEntity->getPos().x, terrainHeight, playerEntity->getPos().z);
            playerEntity->setPos(newPos);
        } else {
            glm::vec3 newPos(playerEntity->getPos().x, playerEntity->getPos().y + jumpingSpeed, playerEntity->getPos().z);
            playerEntity->setPos(newPos);
        }
        jumpingSpeed += GRAVITY.y;
    } else if(playerEntity->getPos().y > 1 + currentTerrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z)) {
        inAir = true;
    } else {
        glm::vec3 terrainHeight(playerEntity->getPos().x, 1 + currentTerrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z), playerEntity->getPos().z);
        playerEntity->setPos(terrainHeight);
    }
}

void Player::movePlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains) {
    setCurrentTerrain(terrains);
    glm::vec3 newRotation(0, -camera->Yaw, 0);
    getPlayerEntity().setRotation(newRotation);
    glm::vec3 moveDir = camera->Front;
    moveDir.y = 0;
    moveDir = glm::normalize(moveDir);
    glm::vec3 forwardMove = currentSpeed * moveDir;
    glm::vec3 lateralMove = lateralSpeed * camera->Right;
    glm::vec3 finalMove = forwardMove + lateralMove;
    glm::vec3 move;
    bool cantGetOver = false;
    float height;
    float currentHeight = playerEntity->getPos().y;
    float dist = std::sqrt(finalMove.x*finalMove.x + finalMove.z*finalMove.z);
    for(int i = 0; i < dist * 3; ++i) {
        move = glm::normalize(finalMove) * (float)i;
        height = playerEntity->getScale().y + currentTerrain->getTerrainHeight(playerEntity->getPos().x + move.x, playerEntity->getPos().z + move.z);
        if(height >= playerEntity->getPos().y + simGravity(i) - 1) {
            cantGetOver = true;
            break;
        }
    }
    for(int i = 0; i < dist; ++i) {
        move = glm::normalize(finalMove) * (float)i;
        height = playerEntity->getScale().y + currentTerrain->getTerrainHeight(playerEntity->getPos().x + move.x, playerEntity->getPos().z + move.z);
        if(height > currentHeight + playerEntity->getScale().y && cantGetOver) {
            finalMove = glm::normalize(finalMove) * (float)(i - 1);
            break;
        } else {
            currentHeight = height;
        }
    }
    collideAndSlide(finalMove, currentGravity, entities);
    if(playerEntity->getPos().y <= currentTerrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z) + 1) {
        glm::vec3 newPos(playerEntity->getPos().x, currentTerrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z) + 1, playerEntity->getPos().z);
        playerEntity->setPos(newPos);
        currentGravity.y = 0;
        inAir = false;
    } else {
        currentGravity.y += GRAVITY.y;
    }
    camera->Position = playerEntity->getPos();
    camera->setYPos(playerEntity->getPos().y + playerEntity->getScale().y + 0.5f);
}

void Player::setCurrentTerrain(std::vector<Terrain *> &terrains) {
    for(Terrain* terrain : terrains) {
        if(playerEntity->getPos().x >= terrain->getPos().x && playerEntity->getPos().x <= terrain->getPos().x + TerrainMesh::SIZE && playerEntity->getPos().z >= terrain->getPos().z && playerEntity->getPos().z <= terrain->getPos().z + TerrainMesh::SIZE) {
            currentTerrain = terrain;
            break;
        }
    }
}

void Player::render(Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor) {
    playerEntity->render(*camera, shader, lightPos, lightColor);
}

void Player::setSpeed(float speed) {
    currentSpeed = speed;
}

void Player::setLateralSpeed(float speed) {
    lateralSpeed = speed;
}

Entity& Player::getPlayerEntity() {
    return *playerEntity;
}

bool Player::isInAir() {
    return inAir;
}

void Player::jump() {
    currentGravity.y = JUMP_POWER;
    inAir = true;
}

float Player::simGravity(float tics) {
    return currentGravity.y + (GRAVITY.y * tics);
}

void Player::calculateCollisions(std::vector<Plane> &planes) {
    //std::vector nearbyPlanes = calculateCollidablePlanes(planes);
    //std::cout << "size: " << nearbyPlanes.size() << std::endl;
    for(const Plane& plane : planes) {
        checkTriangle(plane);
    }
}

void Player::checkTriangle(const Plane &trianglePlane) {
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
            glm::vec3 planeIntersectionPoint = (move.eSpaceStartingPos - trianglePlane.normal + (float)t0 * move.eSpaceMovement);
            if (checkPointInTriangle(planeIntersectionPoint, trianglePlane.points[0], trianglePlane.points[1], trianglePlane.points[2])) {
                foundCollison = true;
                t = t0;
                collisionPoint = planeIntersectionPoint;
            }
        }
        if (!foundCollison) {
            glm::vec3 velocity = move.eSpaceMovement;
            glm::vec3 base = move.eSpaceStartingPos;
            float velocitySquaredLength = velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z;
            float a,b,c;
            float newT;
            a = velocitySquaredLength;
// P1
            b = 2.0*(glm::dot(base-trianglePlane.points[0], velocity));
            glm::vec3 cTemp = trianglePlane.points[0] - base;
            c = cTemp.x * cTemp.x + cTemp.y * cTemp.y + cTemp.z * cTemp.z - 1.0;
            if (getLowestRoot(a,b,c, t, &newT)) {
                t = newT;
                foundCollison = true;
                collisionPoint = trianglePlane.points[0];
            }
// P2
            b = 2.0*(glm::dot(base-trianglePlane.points[1], velocity));
            cTemp = trianglePlane.points[1] - base;
            c = cTemp.x * cTemp.x + cTemp.y * cTemp.y + cTemp.z * cTemp.z - 1.0;
            if (getLowestRoot(a,b,c, t, &newT)) {
                t = newT;
                foundCollison = true;
                collisionPoint = trianglePlane.points[1];
            }
// P3
            b = 2.0*(glm::dot(base-trianglePlane.points[2], velocity));
            cTemp = trianglePlane.points[2] - base;
            c = cTemp.x * cTemp.x + cTemp.y * cTemp.y + cTemp.z * cTemp.z - 1.0;
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
            b = edgeSquaredLength*(2 * glm::dot(baseToVertex, velocity)) - 2.0*edgeDotVelocity*edgeDotBaseToVertex;
            c = edgeSquaredLength*(1 - (baseToVertex.x * baseToVertex.x + baseToVertex.y * baseToVertex.y + baseToVertex.z * baseToVertex.z)) + edgeDotBaseToVertex*edgeDotBaseToVertex;
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
            a = edgeSquaredLength*-velocitySquaredLength +
                edgeDotVelocity*edgeDotVelocity;
            b = edgeSquaredLength*(2 * glm::dot(baseToVertex, velocity))-2.0*edgeDotVelocity*edgeDotBaseToVertex;
            c = edgeSquaredLength*(1-(baseToVertex.x * baseToVertex.x + baseToVertex.y * baseToVertex.y + baseToVertex.z * baseToVertex.z))+edgeDotBaseToVertex*edgeDotBaseToVertex;
            if (getLowestRoot(a,b,c, t, &newT)) {
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
            b = edgeSquaredLength*(2*glm::dot(baseToVertex, velocity)) - 2.0*edgeDotVelocity*edgeDotBaseToVertex;
            c = edgeSquaredLength*(1-(baseToVertex.x * baseToVertex.x + baseToVertex.y * baseToVertex.y + baseToVertex.z * baseToVertex.z)) + edgeDotBaseToVertex*edgeDotBaseToVertex;
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
            float distToCollision = t * move.eSpaceMovement.length();
// Does this triangle qualify for the closest hit?
// it does if it’s the first hit or the closest
            if (!move.foundCollision || distToCollision < move.nearestDistance) {
// Collision information nessesary for sliding
                move.nearestDistance = distToCollision;
                move.intersectionPoint = collisionPoint;
                move.foundCollision = true;
            }
        }
    }
}

std::vector<Plane> Player::calculateCollidablePlanes(std::vector<Plane> &planes) {
    std::vector<Plane> nearbyPlanes;
    for(const Plane& plane : planes) {
        if(plane.origin.x < playerEntity->getPos().x + 10 && plane.origin.x > playerEntity->getPos().x - 10 && plane.origin.y < playerEntity->getPos().y + 10 && plane.origin.y > playerEntity->getPos().y - 10 && plane.origin.z < playerEntity->getPos().z + 10 && plane.origin.z > playerEntity->getPos().z - 10) {
            nearbyPlanes.push_back(plane);
        }
    }
    return nearbyPlanes;
}

typedef unsigned int uint32;
#define in(a) ((uint32&) a)
bool Player::checkPointInTriangle(const glm::vec3& point, const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc)
{
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

void Player::collideAndSlide(const glm::vec3& vel, const glm::vec3& gravity, std::vector<Entity*> &entities)
{
    move.startingPos = playerEntity->getPos();
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
    playerEntity->setPos(finalPosition);
}

glm::vec3 Player::collideWithWorld(const glm::vec3& pos, const glm::vec3& vel, std::vector<Entity*> &entities) {
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
        calculateCollisions(entity->planes);
    }
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
    glm::vec3 newDestinationPoint = destinationPoint - (float)slidingPlane.signedDistanceTo(destinationPoint) * slidePlaneNormal;
    glm::vec3 newVelocityVector = newDestinationPoint - move.intersectionPoint;

    if (std::sqrt(std::pow(newVelocityVector.x, 2) + std::pow(newVelocityVector.y, 2) + std::pow(newVelocityVector.z, 2)) < veryCloseDistance) {
        return newBasePoint;
    }

    ++collisionRecursionDepth;
    return collideWithWorld(newBasePoint, newVelocityVector, entities);
}

bool Player::getLowestRoot(float a, float b, float c, float maxR, float* root) {
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


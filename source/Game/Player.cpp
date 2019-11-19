#include "Headers/Game/Player.h"

Player::Player() = default;

Player::Player(Terrain *terrain1, Camera *camera1, Entity *container1) {
    terrain = terrain1;
    camera = camera1;
    playerEntity = container1;
    move.eRadius = playerEntity->getScale();
}

void Player::setHeight(){
    if(inAir) {
        float terrainHeight = 1 + terrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z);
        if(playerEntity->getPos().y + jumpingSpeed <= terrainHeight) {
            inAir = false;
            jumpingSpeed = 0;
            glm::vec3 newPos(playerEntity->getPos().x, terrainHeight, playerEntity->getPos().z);
            playerEntity->setPos(newPos);
        } else {
            glm::vec3 newPos(playerEntity->getPos().x, playerEntity->getPos().y + jumpingSpeed, playerEntity->getPos().z);
            playerEntity->setPos(newPos);
        }
        //jumpingSpeed += GRAVITY;
    } else if(playerEntity->getPos().y > 1 + terrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z)) {
        inAir = true;
    } else {
        glm::vec3 terrainHeight(playerEntity->getPos().x, 1 + terrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z), playerEntity->getPos().z);
        playerEntity->setPos(terrainHeight);
    }
}

void Player::movePlayer(std::vector<Entity> &entities) {
    glm::vec3 newRotation(0, -camera->Yaw, 0);
    getPlayerEntity().setRotation(newRotation);
    glm::vec3 moveDir = camera->Front;
    moveDir.y = 0;
    moveDir = glm::normalize(moveDir);
    glm::vec3 forwardMove = currentSpeed * moveDir;
    glm::vec3 lateralMove = lateralSpeed * camera->Right;
    glm::vec3 finalMove = forwardMove + lateralMove;
    glm::vec3 move;
    float currentHeight = playerEntity->getPos().y;
    float dist = std::sqrt(finalMove.x*finalMove.x + finalMove.z*finalMove.z);
    for(int i = 0; i < dist; ++i) {
        move = glm::normalize(finalMove) * (float)i;
        float height = 1 + terrain->getTerrainHeight(playerEntity->getPos().x + move.x, playerEntity->getPos().z + move.z);
        if(height > currentHeight + 1) {
            finalMove = glm::normalize(finalMove) * (float)(i - 1);
            break;
        } else {
            currentHeight = height;
        }
    }
    playerEntity->translate(finalMove);
    setHeight();
    camera->Position = playerEntity->getPos();
    camera->setYPos(playerEntity->getPos().y + 1.5f);
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
    jumpingSpeed = JUMP_POWER;
    inAir = true;
}

void Player::calculateCollisions(std::vector<Plane> &planes) {
    std::vector nearbyPlanes = calculateCollidablePlanes(planes);
    std::cout << nearbyPlanes.size() << std::endl;
    for(const Plane& plane : nearbyPlanes) {
        checkTriangle(plane);
    }
}

void Player::checkTriangle(const Plane &trianglePlane) {
// Is triangle front-facing to the velocity vector?
// We only check front-facing triangles
// (your choice of course)
    if (trianglePlane.isFrontFacingTo(move.eSpaceMovementNormalized)) {
// Get interval of plane intersection:
        double t0, t1;
        bool embeddedInPlane = false;
// Calculate the signed distance from sphere
// position to triangle plane
        double signedDistToTrianglePlane =
                trianglePlane.signedDistanceTo(move.eSpaceStartingPos);
// cache this as we’re going to use it a few times below:
        float normalDotVelocity =
                glm::dot(trianglePlane.normal, move.eSpaceMovement);
// if sphere is travelling parrallel to the plane:
        if (normalDotVelocity == 0.0f) {
            if (fabs(signedDistToTrianglePlane) >= 1.0f) {
// Sphere is not embedded in plane.
// No collision possible:
                return;
            } else {
// sphere is embedded in plane.
// It intersects in the whole range [0..1]
                embeddedInPlane = true;
                t0 = 0.0;
                t1 = 1.0;
            }
        } else {
// N dot D is not 0. Calculate intersection interval:
            t0 = (-1.0 - signedDistToTrianglePlane) / normalDotVelocity;
            t1 = (1.0 - signedDistToTrianglePlane) / normalDotVelocity;
// Swap so t0 < t1
            if (t0 > t1) {
                double temp = t1;
                t1 = t0;
                t0 = temp;
            }
// Check that at least one result is within range:
            if (t0 > 1.0f || t1 < 0.0f) {
// Both t values are outside values [0,1]
// No collision possible:
                return;
            }
// Clamp to [0,1]
            if (t0 < 0.0) t0 = 0.0;
            if (t1 < 0.0) t1 = 0.0;
            if (t0 > 1.0) t0 = 1.0;
            if (t1 > 1.0) t1 = 1.0;
        }
// OK, at this point we have two time values t0 and t1
// between which the swept sphere intersects with the
// triangle plane. If any collision is to occur it must
// happen within this interval.
        glm::vec3 collisionPoint;
        bool foundCollison = false;
        float t = 1.0;
// First we check for the easy case - collision inside
// the triangle. If this happens it must be at time t0
// as this is when the sphere rests on the front side
// of the triangle plane. Note, this can only happen if
// the sphere is not embedded in the triangle plane.
        std::cout << "embeddedInPlane: " << !embeddedInPlane << std::endl;
        if (!embeddedInPlane) {
            glm::vec3 planeIntersectionPoint = (move.eSpaceStartingPos - trianglePlane.normal + (float)t0 * move.eSpaceMovement);
            if (checkPointInTriangle(planeIntersectionPoint, trianglePlane.points[0], trianglePlane.points[1], trianglePlane.points[2])) {
                foundCollison = true;
                t = t0;
                collisionPoint = planeIntersectionPoint;
            }
        }
        std::cout << "foundCollision: " << foundCollison << std::endl;
        // if we haven’t found a collision already we’ll have to
// sweep sphere against points and edges of the triangle.
// Note: A collision inside the triangle (the check above)
// will always happen before a vertex or edge collision!
// This is why we can skip the swept test if the above
// gives a collision!
        if (!foundCollison) {
// some commonly used terms:
            glm::vec3 velocity = move.eSpaceMovement;
            glm::vec3 base = move.eSpaceStartingPos;
            float velocitySquaredLength = velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z;
            float a,b,c; // Params for equation
            float newT;
// For each vertex or edge a quadratic equation have to
// be solved. We parameterize this equation as
// a*t^2 + b*t + c = 0 and below we calculate the
// parameters a,b and c for each test.
// Check against points:
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
        if (foundCollison) {
// distance to collision: ’t’ is time of collision
            float distToCollision = t * move.eSpaceMovement.length();
// Does this triangle qualify for the closest hit?
// it does if it’s the first hit or the closest
            if (!move.foundCollision || distToCollision < move.nearestDistance) {
// Collision information nessesary for sliding
                std::cout << "Collision Found" << std::endl;
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
        if(plane.origin.x < playerEntity->getPos().x + 5 && plane.origin.x > playerEntity->getPos().x - 5 && plane.origin.z < playerEntity->getPos().z + 5 && plane.origin.z > playerEntity->getPos().z - 5) {
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

void Player::collideAndSlide(const glm::vec3& vel, const glm::vec3& gravity, std::vector<Entity> &entities)
{
// Do collision detection:
    move.startingPos = playerEntity->getPos();
    move.movement = vel;
// calculate position and velocity in eSpace
    glm::vec3 eSpacePosition = move.startingPos / move.eRadius;
    glm::vec3 eSpaceVelocity = move.movement / move.eRadius;
// Iterate until we have our final position.
    collisionRecursionDepth = 0;
    glm::vec3 finalPosition = collideWithWorld(eSpacePosition, eSpaceVelocity, entities);
// Add gravity pull:
// To remove gravity uncomment from here .....
// Set the new R3 position (convert back from eSpace to R3
    move.startingPos = finalPosition * move.eRadius;
    move.movement = gravity;
    eSpaceVelocity = gravity / move.eRadius;
    collisionRecursionDepth = 0;
    finalPosition = collideWithWorld(finalPosition, eSpaceVelocity, entities);
    std::cout << "X: " << finalPosition.x << ", Y: " << finalPosition.y << ", Z: " << finalPosition.z << std::endl;
// ... to here
// Convert final result back to R3:
    finalPosition *= move.eRadius;
// Move the entity (application specific function)
    playerEntity->setPos(finalPosition);
}

glm::vec3 Player::collideWithWorld(const glm::vec3& pos, const glm::vec3& vel, std::vector<Entity> &entities) {
// All hard-coded distances in this function is
// scaled to fit the setting above..
    float unitScale = unitsPerMeter / 100.0f;
    float veryCloseDistance = 0.005f * unitScale;
// do we need to worry?
    if (collisionRecursionDepth>5)
        return pos;
// Ok, we need to worry:
    move.eSpaceMovement = vel;
    move.eSpaceMovementNormalized = glm::normalize(vel);
    move.eSpaceStartingPos = pos;
    move.foundCollision = false;
// Check for collision (calls the collision routines)
    for(Entity entity : entities) {
        calculateCollisions(entity.planes);
    }

// If no collision we just move along the velocity
    std::cout << "move.collision: " << move.foundCollision << std::endl;
    if (!move.foundCollision) {
        std::cout << "no collision found" << std::endl;
        return pos + vel;
    }
// *** Collision occured ***
// The original destination point
    glm::vec3 destinationPoint = pos + vel;
    glm::vec3 newBasePoint = pos;
// only update if we are not already very close
// and if so we only move very close to intersection..not
// to the exact spot.
    if (move.nearestDistance>=veryCloseDistance)
    {
        glm::vec3 V = vel;
        V = glm::normalize(V) * (float)(move.nearestDistance - veryCloseDistance);
        newBasePoint = move.eSpaceStartingPos + V;
// Adjust polygon intersection point (so sliding
// plane will be unaffected by the fact that we
// move slightly less than collision tells us)
        V = glm::normalize(V);
        move.intersectionPoint -= veryCloseDistance * V;
    }
// Determine the sliding plane
    glm::vec3 slidePlaneOrigin = move.intersectionPoint;
    glm::vec3 slidePlaneNormal = newBasePoint-move.intersectionPoint;
    slidePlaneNormal = glm::normalize(slidePlaneNormal);
    Plane slidingPlane(slidePlaneOrigin,slidePlaneNormal);
// Again, sorry about formatting.. but look carefully ;)
    glm::vec3 newDestinationPoint = destinationPoint - (float)slidingPlane.signedDistanceTo(destinationPoint) * slidePlaneNormal;
// Generate the slide vector, which will become our new
// velocity vector for the next iteration
    glm::vec3 newVelocityVector = newDestinationPoint - move.intersectionPoint;
// Recurse:
// dont recurse if the new velocity is very small
    if (std::sqrt(std::pow(newVelocityVector.x, 2) + std::pow(newVelocityVector.y, 2) + std::pow(newVelocityVector.z, 2)) < veryCloseDistance) {
        return newBasePoint;
    }
    ++collisionRecursionDepth;
    return collideWithWorld(newBasePoint, newVelocityVector, entities);
}

bool Player::getLowestRoot(float a, float b, float c, float maxR, float* root) {
// Check if a solution exists
    float determinant = b * b - 4.0f * a * c;
// If determinant is negative it means no solutions.
    if (determinant < 0.0f) return false;
// calculate the two roots: (if determinant == 0 then
// x1==x2 but let’s disregard that slight optimization)
    float sqrtD = sqrt(determinant);
    float r1 = (-b - sqrtD) / (2 * a);
    float r2 = (-b + sqrtD) / (2 * a);
// Sort so x1 <= x2
    if (r1 > r2) {
        float temp = r2;
        r2 = r1;
        r1 = temp;
    }
// Get lowest root:
    if (r1 > 0 && r1 < maxR) {
        *root = r1;
        return true;
    }
// It is possible that we want x2 - this can happen
// if x1 < 0
    if (r2 > 0 && r2 < maxR) {
        *root = r2;
        return true;
    }
// No (valid) solutions
    return false;
}


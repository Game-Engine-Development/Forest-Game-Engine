#include "Headers/Game/Player.h"

Player::Player() = default;

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
        jumpingSpeed += GRAVITY;
    } else if(playerEntity->getPos().y > 1 + terrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z)) {
        inAir = true;
    } else {
        glm::vec3 terrainHeight(playerEntity->getPos().x, 1 + terrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z), playerEntity->getPos().z);
        playerEntity->setPos(terrainHeight);
    }
}

void Player::move() {
    glm::vec3 newRotation(0, -camera->Yaw, 0);
    getPlayerEntity().setRotation(newRotation);
    glm::vec3 moveDir = camera->Front;
    moveDir.y = 0;
    moveDir = glm::normalize(moveDir);
    glm::vec3 forwardMove = currentSpeed * moveDir;
    glm::vec3 lateralMove = lateralSpeed * camera->Right;
    glm::vec3 finalMove = forwardMove + lateralMove;
    Movement move(finalMove, *this);
    calculateCollisions(terrain->getTerrainMesh().planes, move);
    //playerEntity->translate(finalMove);
    //setHeight();
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

void Player::calculateCollisions(std::vector<Plane> planes, Movement &move) {
    std::vector<Plane> nearbyPlanes = calculateCollidablePlanes(planes);
    //radius == container->getScale().y. radius /= container->getScale().y/2.0f
    //collision:
    glm::vec3 v(1.0f, 1.0f, 1.0f);
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;
    glm::vec3 radiusVector(playerEntity->getScale().x, playerEntity->getScale().y, playerEntity->getScale().z);
    float e1 = (1.0f/playerEntity->getPos().x)*v1 + 0.0f*v2 + 0.0f*v3;
    float e2 = 0.0f*v1 + (1.0f/playerEntity->getPos().y)*v2 + 0.0f*v3;
    float e3 = 0.0f*v1 + 0.0f*v2 + (1.0f/playerEntity->getPos().z)*v3;
    glm::mat3 CBM(glm::vec3((1.0f/playerEntity->getPos().x), 0, 0), glm::vec3(0, (1.0f/playerEntity->getPos().y), 0), glm::vec3(0, 0, (1.0f/playerEntity->getPos().z)));
    glm::vec3 ve = CBM * v;
    assert(ve == glm::vec3(e1, e2, e3));
    for(Plane plane : nearbyPlanes) {
        checkTriangle(plane, move);
    }
}

void Player::checkTriangle(Plane &trianglePlane, Movement &move) {
// Is triangle front-facing to the velocity vector?
// We only check front-facing triangles
// (your choice of course)
    if (trianglePlane.isFrontFacingTo(
            move.eSpaceMovementNormalized)) {
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
        if (!embeddedInPlane) {
            glm::vec3 planeIntersectionPoint = (move.eSpaceStartingPos - trianglePlane.normal + (float)t0 * move.eSpaceMovement);
            if (checkPointInTriangle(planeIntersectionPoint, trianglePlane.points[0], trianglePlane.points[1], trianglePlane.points[2])) {
                foundCollison = true;
                t = t0;
                collisionPoint = planeIntersectionPoint;
            }
        }
// Set result:
        if (foundCollison == true) {
// distance to collision: ’t’ is time of collision
            float distToCollision = t * move.eSpaceMovement.length();
// Does this triangle qualify for the closest hit?
// it does if it’s the first hit or the closest
            if (move.foundCollision == false ||
                distToCollision < move.nearestDistance) {
// Collision information nessesary for sliding
                move.nearestDistance = distToCollision;
                move.intersectionPoint = collisionPoint;
                move.foundCollision = true;
            }
        }
    }
}

std::vector<Plane> Player::calculateCollidablePlanes(std::vector<Plane> &planes) {
    return planes;
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

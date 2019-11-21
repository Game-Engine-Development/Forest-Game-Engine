#pragma once

#include <glm/glm.hpp>
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Camera.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Shader.h"
#include <array>
#include "Test/Plane.h"

class Player {
private:
    Terrain *terrain;
    Camera *camera;
    Entity *playerEntity;
    float currentSpeed = 0;
    float lateralSpeed = 0;
    float jumpingSpeed = 0;
    bool inAir = false;
    glm::vec3 currentGravity = glm::vec3(0,0,0);
    static constexpr glm::vec3 GRAVITY = glm::vec3(0, -0.08f, 0);
    static constexpr float unitsPerMeter = 100;
    std::vector<Plane> calculateCollidablePlanes(std::vector<Plane>& planes);
    bool getLowestRoot(float a, float b, float c, float maxR, float* root);
    bool checkPointInTriangle(const glm::vec3& point, const glm::vec3& pa,const glm::vec3& pb, const glm::vec3& pc);
    struct Movement {
        Movement() = default;
        Movement(glm::vec3 &mov, Player &player) {
            movement = mov;
            eRadius = player.getPlayerEntity().getScale();
            startingPos = player.getPlayerEntity().getPos();
            eSpaceMovement = mov / eRadius;
            eSpaceMovementNormalized = glm::normalize(eSpaceMovement);
            eSpaceStartingPos = startingPos / eRadius;
        }
        glm::vec3 eRadius;
        glm::vec3 movement;
        glm::vec3 startingPos;
        glm::vec3 eSpaceMovement;
        glm::vec3 eSpaceMovementNormalized;
        glm::vec3 eSpaceStartingPos;

        bool foundCollision;
        double nearestDistance;
        glm::vec3 intersectionPoint;
    };
    int collisionRecursionDepth = 0;
    Movement move;
    void checkTriangle(const Plane &trianglePlane);
    void collideAndSlide(const glm::vec3& vel, const glm::vec3& gravity, std::vector<Entity*>& entities);
    glm::vec3 collideWithWorld(const glm::vec3& pos, const glm::vec3& vel, std::vector<Entity*>& entities);
public:
    static constexpr float SPEED = 2.0f;
    static constexpr float LATERAL_SPEED = 1.5f;
    static constexpr float JUMP_POWER = 2.0f;
    Player();
    explicit Player(Terrain *terrain1, Camera *camera1, Entity *container1);
    void setHeight();
    void movePlayer(std::vector<Entity*>& entities);
    void render(Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor);
    void calculateCollisions(std::vector<Plane>& planes);
    void setSpeed(float speed);
    void setLateralSpeed(float speed);
    Entity& getPlayerEntity();
    bool isInAir();
    void jump();
};

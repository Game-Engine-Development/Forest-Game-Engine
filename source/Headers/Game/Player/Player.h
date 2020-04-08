#pragma once

#include <glm/glm.hpp>
#include <array>

#include "Headers/Engine/Collisions/CollisionHandler.h"
#include "Headers/Engine/Collisions/Plane.h"
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Shader/Shader.h"

class Player {
private:
    Terrain *currentTerrain;
    Camera *camera;
    Entity *playerEntity;
    CollisionHandler handler;
    float currentSpeed = 0;
    float lateralSpeed = 0;
    float jumpingSpeed = 0;
    int currentHealth = MAX_HEALTH;
public:
    static constexpr float SPEED = 2.0f;
    static constexpr float LATERAL_SPEED = 1.5f;
    static constexpr float JUMP_POWER = 2.0f;
    static constexpr int MAX_HEALTH = 20;

    Player();
    explicit Player(Camera *camera1, Entity *container1, CollisionHandler &collisionHandler);

    template<size_t N>
    void movePlayer(std::vector<Entity*> &entities, std::array<Terrain, N> &terrains, Entity* boundingBox, bool bound) {
        glm::vec3 newRotation(0, -camera->Yaw, 0);
        playerEntity->setRotation(newRotation);
        glm::vec3 finalMove = camera->Front;
        finalMove.y = 0;
        finalMove = glm::normalize(finalMove);
        finalMove *= currentSpeed;
        finalMove += lateralSpeed * camera->Right;
        handler.moveEntity(finalMove, entities, terrains, boundingBox, bound);
        camera->Position = playerEntity->getPos();
        camera->setYPos(playerEntity->getPos().y + 4);
    }

    void render(Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor);
    void setSpeed(float speed);
    void setLateralSpeed(float speed);
    Entity& getPlayerEntity();
    bool isInAir();
    void jump();
    void takeDamage(int damage);
    float getHealth();

    float getCurrentHealth();
};

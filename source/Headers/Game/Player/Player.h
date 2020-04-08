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
    float currentHealth = MAX_HEALTH;
public:
    static constexpr float SPEED = 2.0f;
    static constexpr float LATERAL_SPEED = 1.5f;
    static constexpr float JUMP_POWER = 2.0f;
    static constexpr int MAX_HEALTH = 20;

    Player();
    explicit Player(Camera *camera1, Entity *container1, CollisionHandler &collisionHandler);

    template<unsigned long N>
    void movePlayer(std::vector<Entity*> &entities, std::array<Terrain, N> &terrains, Entity* boundingBox, bool bound) {
        glm::vec3 newRotation(0, -camera->Yaw, 0);
        playerEntity->setRotation(newRotation);
        glm::vec3 finalMove = camera->Front;
        finalMove.y = 0;
        finalMove = glm::normalize(finalMove);
        finalMove *= currentSpeed;
        finalMove += lateralSpeed * camera->Right;
        handler.moveEntity(finalMove, entities, terrains);
        camera->Position = playerEntity->getPos();
        camera->setYPos(playerEntity->getPos().y + 4);
        if(bound && boundingBox != nullptr) {
            if(playerEntity->getPos().x > boundingBox->getScale().x - 5) {
                glm::vec3 newPos(boundingBox->getScale().x - 5, playerEntity->getPos().y, playerEntity->getPos().z);
                playerEntity->setPos(newPos);
            } else if(playerEntity->getPos().x < -boundingBox->getScale().x + 5) {
                glm::vec3 newPos(-boundingBox->getScale().x + 5, playerEntity->getPos().y, playerEntity->getPos().z);
                playerEntity->setPos(newPos);
            }
            if(playerEntity->getPos().z > boundingBox->getScale().z - 5) {
                glm::vec3 newPos(playerEntity->getPos().x, playerEntity->getPos().y, boundingBox->getScale().z - 5);
                playerEntity->setPos(newPos);
            } else if(playerEntity->getPos().z < -boundingBox->getScale().z + 5) {
                glm::vec3 newPos(playerEntity->getPos().x, playerEntity->getPos().y, -boundingBox->getScale().z + 5);
                playerEntity->setPos(newPos);
            }
        }
    }

    void render(Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor);
    void setSpeed(float speed);
    void setLateralSpeed(float speed);
    Entity& getPlayerEntity();
    bool isInAir();
    void jump();
    void takeDamage(float damage);
    float getHealth();

    float getCurrentHealth();
};

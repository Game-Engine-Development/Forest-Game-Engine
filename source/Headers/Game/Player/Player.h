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
    Camera *camera;
    Entity *playerEntity;
    CollisionHandler handler;
    float currentSpeed = 0;
    float lateralSpeed = 0;

public:
    static constexpr float SPEED = 2.0f;
    static constexpr float LATERAL_SPEED = 1.5f;
    static constexpr float JUMP_POWER = 2.0f;

    explicit Player(Camera *camera1, Entity *container1, CollisionHandler &collisionHandler);

    template<size_t N>
    void movePlayer(std::vector<Entity> &entities, std::array<Terrain, N> &terrains);

    void render(const Shader &shader, glm::vec3 lightPos, glm::vec3 lightColor) const;
    void setSpeed(float speed);
    void setLateralSpeed(float speed);
    [[nodiscard]] bool isInAir() const noexcept;
    void jump();
};

//definitions of template functions:

template<size_t N>
void Player::movePlayer(std::vector<Entity> &entities, std::array<Terrain, N> &terrains) {
    glm::vec3 newRotation(0, -camera->getYaw(), 0);
    playerEntity->setRotation(newRotation);
    glm::vec3 finalMove = camera->getFront();
    finalMove.y = 0;
    finalMove = glm::normalize(finalMove);
    finalMove *= currentSpeed;
    finalMove += lateralSpeed * camera->getRight();
    handler.moveEntity(finalMove, entities, terrains);
    camera->setPosition(playerEntity->getPos());
    //camera->setPosition(glm::vec3(playerEntity->getPos().x, playerEntity->getPos().y + 20.f, playerEntity->getPos().z));
}

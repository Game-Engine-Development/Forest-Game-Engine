#pragma once

#include <glm/glm.hpp>
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Shader/Shader.h"
#include <array>
#include <Headers/Engine/Collisions/CollisionHandler.h>
#include "Headers/Engine/Collisions/Plane.h"

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
    void movePlayer(std::vector<Entity*>& entities, std::vector<Terrain*>& terrains, Entity* boundingBox, bool bound);
    void render(Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor);
    void setSpeed(float speed);
    void setLateralSpeed(float speed);
    Entity& getPlayerEntity();
    bool isInAir();
    void jump();
    void takeDamage(float damage);
    float getHealth();
};

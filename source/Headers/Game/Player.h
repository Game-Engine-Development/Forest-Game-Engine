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
    static constexpr float GRAVITY = -0.08f;
    std::vector<Plane> calculateCollidablePlanes(std::vector<Plane>& planes);
public:
    static constexpr float SPEED = 4.0f;
    static constexpr float LATERAL_SPEED = 2.0f;
    static constexpr float JUMP_POWER = 2.0f;
    Player();
    explicit Player(Terrain *terrain1, Camera *camera1, Entity *container1) : terrain(terrain1), camera(camera1), playerEntity(container1){};
    void setHeight();
    void move();
    void render(Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor);
    void calculateCollisions(std::vector<Plane>& planes);
    void setSpeed(float speed);
    void setLateralSpeed(float speed);
    Entity& getPlayerEntity();
    bool isInAir();
    void jump();
};

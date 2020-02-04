#pragma once

#include <glm/glm.hpp>
#include <Headers/Engine/Camera.h>
#include <Headers/Engine/Models/Entity.h>
#include "Player.h"

class Shooter {
public:
    Shooter(Camera* camera, Entity* entity, Player* player);
    glm::vec3 getCurrentRay();
    void update();
    void shoot(std::vector<Entity*>& entities, std::vector<Terrain*>& terrains);
private:
    glm::vec3 m_currentRay;
    Camera* m_camera;
    Entity* m_bulletEntity;
    Player* m_player;
    CollisionHandler m_handler;

    void calculateCurrentRay();
    glm::vec2 getNormalizedDeviceCoords(glm::vec2 mouseCoords);
    glm::vec4 toEyeCoords(glm::vec4 clipCoords);
    glm::vec3 toWorldCoords(glm::vec4 eyeCoords);
};

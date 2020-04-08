#pragma once

#include <array>
#include <glm/glm.hpp>

#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Models/Entity.h"
#include "Player.h"

class Shooter {
public:
    Shooter(Camera* camera, Entity* entity, Player* player);
    glm::vec3 getCurrentRay();
    void update();

    template <size_t N>
    void shoot(std::vector<Entity*>& entities, std::array<Terrain, N>& terrains) {
        glm::vec3 newPos(m_camera->getPos());
        m_bulletEntity->setPos(newPos);
        glm::vec3 shotMove(m_currentRay * 1000.0f);
        m_handler.moveEntityWithoutGravity(shotMove, entities, terrains);
    }
private:
    glm::vec3 m_currentRay;
    Camera* m_camera;
    Entity* m_bulletEntity;
    Player* m_player;
    CollisionHandler m_handler;

    void calculateCurrentRay();
    static glm::vec2 getNormalizedDeviceCoords(glm::vec2 mouseCoords);
    glm::vec4 toEyeCoords(glm::vec4 clipCoords);
    glm::vec3 toWorldCoords(glm::vec4 eyeCoords);
};

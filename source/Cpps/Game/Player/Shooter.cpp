#include "Headers/Game/Player/Shooter.h"

Shooter::Shooter(Camera *camera, Entity* entity, Player* player)
        : m_camera(camera), m_bulletEntity(entity), m_player(player) {
    m_handler = CollisionHandler(m_bulletEntity);
    update();
}

glm::vec3 Shooter::getCurrentRay() {
    return m_currentRay;
}

void Shooter::update() {
    calculateCurrentRay();
}

void Shooter::calculateCurrentRay() {
    /*glm::vec2 mouseCoords(0, 0);
    mouseCoords = getNormalizedDeviceCoords(mouseCoords);
    glm::vec4 coords(mouseCoords.x, mouseCoords.y, -1.0, 1.0);
    coords = toEyeCoords(coords);
    m_currentRay = toWorldCoords(coords);*/
    m_currentRay = m_camera->Front;

}

glm::vec3 Shooter::toWorldCoords(glm::vec4 eyeCoords) {
    glm::mat4 invertedView = glm::inverse(m_camera->getViewMatrix());
    glm::vec4 worldCoords = invertedView * eyeCoords;
    glm::vec3 ray(worldCoords.x, worldCoords.y, worldCoords.z);
    glm::normalize(ray);
    return ray;
}

glm::vec4 Shooter::toEyeCoords(glm::vec4 clipCoords) {
    glm::mat4 invertedProjection = glm::inverse(m_camera->getProjectionMatrix());
    glm::vec4 eyeCoords = invertedProjection * clipCoords;
    eyeCoords.z = -1.0;
    eyeCoords.w = 0.0;
    return eyeCoords;
}

glm::vec2 Shooter::getNormalizedDeviceCoords(glm::vec2 mouseCoords) {
    float x = (2.0f * mouseCoords.x); //add division
    float y = (2.0f * mouseCoords.y); //add division
    return glm::vec2(x, -y);
}
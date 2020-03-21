#include "Headers/Game/Entities/Spirit.h"

Spirit::Spirit(Entity &entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {}

Spirit::Spirit(Entity &&entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {}

Entity Spirit::getEntity() {
    return m_entity;
}

Entity* Spirit::getEntityPointer() {
    return &m_entity;
}

void Spirit::render(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor) {
    m_entity.render(camera, shader, lightPos, lightColor);
}

void Spirit::hitPlayer() {
    m_player->takeDamage(static_cast<float>(m_damage));
}

void Spirit::takeDamage(int damage) {
    if(m_numOfAnimals <= 0) {
        m_health -= damage;
    }
}

void Spirit::animalDied() {
    m_numOfAnimals -= 1;
}
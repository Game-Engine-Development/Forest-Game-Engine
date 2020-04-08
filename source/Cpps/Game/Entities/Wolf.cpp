#include "Headers/Game/Entities/Wolf.h"

Wolf::Wolf(Entity &entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {

}

Wolf::Wolf(Entity &&entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {

}

Wolf::Wolf(Wolf &wolf) : m_entity(wolf.getEntity()), m_collisionHandler(CollisionHandler(&m_entity)), m_player(wolf.m_player) {

}

Entity Wolf::getEntity() {
    return m_entity;
}

Entity* Wolf::getEntityPointer() {
    return &m_entity;
}

void Wolf::render(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor) {
    m_entity.render(camera, shader, lightPos, lightColor);
}

void Wolf::hitPlayer() {
    m_player->takeDamage(m_damage);
}

void Wolf::takeDamage(int damage) {
    m_health -= damage;
}

bool Wolf::isDead() {
    return m_dead;
}
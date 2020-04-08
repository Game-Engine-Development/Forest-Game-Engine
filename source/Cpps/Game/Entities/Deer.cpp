#include "Headers/Game/Entities/Deer.h"

Deer::Deer(Entity &entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {

}

Deer::Deer(Entity &&entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {

}

Deer::Deer(Deer &deer) : m_entity(deer.getEntity()), m_collisionHandler(CollisionHandler(&m_entity)), m_player(deer.m_player) {

}

Entity Deer::getEntity() {
    return m_entity;
}

Entity* Deer::getEntityPointer() {
    return &m_entity;
}

void Deer::render(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor) {
    m_entity.render(camera, shader, lightPos, lightColor);
}

void Deer::hitPlayer() {
    m_player->takeDamage(m_damage);
}

void Deer::takeDamage(int damage) {
    m_health -= damage;
}

bool Deer::isDead() {
    return m_dead;
}
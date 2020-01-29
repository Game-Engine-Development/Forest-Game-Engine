#include "Headers/Game/Entities/Wolf.h"

Wolf::Wolf(Entity &entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {

}

Wolf::Wolf(Entity &&entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {

}

void Wolf::update(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor, std::vector<Entity *> &entities, std::vector<Terrain *> &terrains) {
    followPlayer(entities, terrains);
    render(camera, shader, lightPos, lightColor);
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

void Wolf::followPlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains) {
    glm::vec3 move = m_player->getPlayerEntity().getPos() - m_entity.getPos();
    if(std::sqrt(move.x*move.x + move.y*move.y + move.z*move.z) < 20 && !m_collisionHandler.inAir) {
        m_collisionHandler.currentGravity.y = JUMP_POWER;
        m_collisionHandler.inAir = true;
    }
    move.y = 0;
    move = glm::normalize(move);
    move *= MOVE_SPEED;
    m_collisionHandler.moveEntity(move, entities, terrains);
}
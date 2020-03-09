#include "Headers/Game/Entities/Wolf.h"

Wolf::Wolf(Entity &entity, Player* player, Spirit* spirit) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player), m_spirit(spirit) {

}

Wolf::Wolf(Entity &&entity, Player* player, Spirit* spirit) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player), m_spirit(spirit) {

}

void Wolf::die() {
    m_spirit->animalDied();
}

void Wolf::update(std::vector<Entity *> &entities, std::vector<Terrain *> &terrains) {
    if(m_health > 0) {
        if (m_entity.hit) {
            takeDamage(1);
            m_entity.hit = false;
            if(m_health <= 0) {
                die();
                for(int i = 0; i < entities.size(); ++i) {
                    if(&m_entity == entities[i]) {
                        entities.erase(entities.begin() + i);
                    }
                }
            }
        }
        followPlayer(entities, terrains);
    } else {
        m_isDead = true;
    }
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
    if(!m_damagedPlayer) {
        if (std::sqrt(move.x * move.x + move.y * move.y + move.z * move.z) < 20 && !m_collisionHandler.inAir) {
            m_collisionHandler.currentGravity.y = JUMP_POWER;
            m_collisionHandler.inAir = true;
        }
        move.y = 0;
        move = glm::normalize(move);
        move *= MOVE_SPEED;
        m_collisionHandler.moveEntity(move, entities, terrains);
        if(m_collisionHandler.hitPlayer) {
            m_damagedPlayer = true;
            hitPlayer();
        }
    } else if(std::sqrt(move.x * move.x + move.y * move.y + move.z * move.z) > 30) {
        m_damagedPlayer = false;
    }
}

void Wolf::hitPlayer() {
    m_player->takeDamage(m_damage);
}

void Wolf::takeDamage(int damage) {
    m_health -= damage;
}

bool Wolf::isDead() {
    return m_isDead;
}
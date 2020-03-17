#include "Headers/Game/Entities/Wolf.h"

Wolf::Wolf(Entity &entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {

}

Wolf::Wolf(Entity &&entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {

}

Wolf::Wolf(Wolf &wolf) : m_entity(wolf.getEntity()), m_collisionHandler(CollisionHandler(&m_entity)), m_player(wolf.m_player) {

}

void Wolf::update(std::vector<Entity *> &entities, std::vector<Terrain *> &terrains, Entity* boundingBox, bool bound) {
    if(m_health > 0) {
        if (m_entity.hit) {
            takeDamage(1);
            m_entity.hit = false;
            if(m_health <= 0) {
                m_dead = true;
                for(int i = 0; i < entities.size(); ++i) {
                    if(entities[i] == &m_entity) {
                        entities.erase(entities.begin() + i);
                    }
                }
            }
        }
        followPlayer(entities, terrains, boundingBox, bound);
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

void Wolf::followPlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains, Entity* boundingBox, bool bound) {
    glm::vec3 move = m_player->getPlayerEntity().getPos() - m_entity.getPos();
    if(!m_damagedPlayer) {
        if (std::sqrt(move.x * move.x + move.y * move.y + move.z * move.z) < 20 && !m_collisionHandler.inAir) {
            m_collisionHandler.currentGravity.y = JUMP_POWER;
            m_collisionHandler.inAir = true;
        }
        move.y = 0;
        move = glm::normalize(move);
        move *= MOVE_SPEED;
        m_collisionHandler.moveEntity(move, entities, terrains, boundingBox, bound);
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
    return m_dead;
}
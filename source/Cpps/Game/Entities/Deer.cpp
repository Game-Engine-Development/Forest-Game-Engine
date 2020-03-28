#include "Headers/Game/Entities/Deer.h"

Deer::Deer(Entity &entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {

}

Deer::Deer(Entity &&entity, Player* player) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player) {

}

Deer::Deer(Deer &deer) : m_entity(deer.getEntity()), m_collisionHandler(CollisionHandler(&m_entity)), m_player(deer.m_player) {

}

void Deer::update(std::vector<Entity *> &entities, std::vector<Terrain *> &terrains, Entity* boundingBox, bool bound) {
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

Entity Deer::getEntity() {
    return m_entity;
}

Entity* Deer::getEntityPointer() {
    return &m_entity;
}

void Deer::render(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor) {
    m_entity.render(camera, shader, lightPos, lightColor);
}

void Deer::followPlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains, Entity* boundingBox, bool bound) {
    glm::vec3 move = m_player->getPlayerEntity().getPos() - m_entity.getPos();
    double turn = glm::degrees(atan(move.x / move.z));
    if(turn > 0) {
        turn -= 180;
    }
    turn += 90;
    glm::vec3 newRotation(m_entity.getRotation().x, turn, m_entity.getRotation().z);
    m_entity.setRotation(newRotation);
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

void Deer::hitPlayer() {
    m_player->takeDamage(m_damage);
}

void Deer::takeDamage(int damage) {
    m_health -= damage;
}

bool Deer::isDead() {
    return m_dead;
}
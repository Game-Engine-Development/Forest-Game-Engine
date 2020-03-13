#include "Headers/Game/Entities/Spirit.h"

Spirit::Spirit(Entity &entity, Player* player, BoundingBox* boundingBox, Wolf& genericWolf, Deer& genericDeer) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player), m_boundingBox(boundingBox), m_genericWolf(genericWolf), m_genericDeer(genericDeer) {

}

void Spirit::update(std::vector<Entity*> &entities, std::vector<Terrain *> &terrains) {
    m_numOfAnimals = m_wolves.size() + m_deer.size();
    if(m_health > 0) {
        if (m_entity.hit) {
            takeDamage(1);
            m_entity.hit = false;
            if(m_health <= 0) {
                m_alive = false;
                releasePlayer(entities);
                for(int i = 0; i < entities.size(); ++i) {
                    if(&m_entity == entities[i]) {
                        entities.erase(entities.begin() + i);
                    }
                }
            }
        }
        followPlayer(entities, terrains);
        if(m_playerBound) {
            m_boundingBox->shrink();
        }
    }
}

Entity Spirit::getEntity() {
    return m_entity;
}

Entity* Spirit::getEntityPointer() {
    return &m_entity;
}

void Spirit::followPlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains) {
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

void Spirit::hitPlayer() {
    m_player->takeDamage(m_damage);
}

void Spirit::takeDamage(int damage) {
    if(m_numOfAnimals <= 0) {
        m_health -= damage;
    }
}

bool Spirit::isBound() {
    return m_playerBound;
}

void Spirit::bindPlayer(std::vector<Entity*>& entities) {
    m_playerBound = true;
    m_boundingBox->getEntity()->setPos(m_player->getPlayerEntity().getPos());
    m_boundingBox->turnOn(entities);
}

void Spirit::releasePlayer(std::vector<Entity *> &entities) {
    m_playerBound = false;
    m_boundingBox->turnOff(entities);
}

void Spirit::spawnDeer(glm::vec3&& pos, std::vector<Entity*>& entities) {
    m_deer.push_back(std::make_shared<Deer>(m_genericDeer));
    m_deer[m_deer.size() - 1]->getEntityPointer()->setPos(pos);
    entities.push_back(m_deer[m_deer.size() - 1]->getEntityPointer());
}

void Spirit::spawnWolf(glm::vec3&& pos, std::vector<Entity*>& entities) {
    m_wolves.push_back(std::make_shared<Wolf>(m_genericWolf));
    m_wolves[m_wolves.size() - 1]->getEntityPointer()->setPos(pos);
    entities.push_back(m_wolves[m_wolves.size() - 1]->getEntityPointer());
}

void Spirit::updateAnimals(std::vector<Entity *> &entities, std::vector<Terrain *> &terrains) {
    for(unsigned int i = 0; i < m_wolves.size(); ++i) {
        m_wolves[i]->update(entities, terrains);
        if(m_wolves[i]->isDead()) {
            m_wolves.erase(m_wolves.begin() + i);
        }
    }
    for(unsigned int i = 0; i < m_deer.size(); ++i) {
        m_deer[i]->update(entities, terrains);
        if(m_deer[i]->isDead()) {
            m_deer.erase(m_deer.begin() + i);
        }
    }
}

void Spirit::spawn(std::vector<Entity*>& entities) {
    m_health = 1;
    m_alive = true;
    int numOfWolves = rand() % 4 + 4;
    for(int i = 0; i < numOfWolves; ++i) {
        spawnWolf(glm::vec3(rand() % BoundingBox::SIZE + m_boundingBox->getEntity()->getPos().x - (BoundingBox::SIZE / 2) , 100, rand() % BoundingBox::SIZE + m_boundingBox->getEntity()->getPos().z - (BoundingBox::SIZE / 2)), entities);
    }
    int numOfDeer = rand() % 4 + 4;
    for(int i = 0; i < numOfDeer; ++i) {
        spawnDeer(glm::vec3(rand() % BoundingBox::SIZE + m_boundingBox->getEntity()->getPos().x - (BoundingBox::SIZE / 2), 100, rand() % BoundingBox::SIZE + m_boundingBox->getEntity()->getPos().z - (BoundingBox::SIZE / 2)), entities);
    }
    bindPlayer(entities);
}

bool Spirit::isAlive() {
    return m_alive;
}

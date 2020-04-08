#include "Headers/Game/Entities/Spirit.h"

Spirit::Spirit(Entity &entity, Player* player, BoundingBox* boundingBox, Animal& genericWolf, Animal& genericDeer) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player), m_boundingBox(boundingBox), m_genericWolf(genericWolf), m_genericDeer(genericDeer) {}

Entity Spirit::getEntity() {
    return m_entity;
}

Entity* Spirit::getEntityPointer() {
    return &m_entity;
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
    m_deer.push_back(std::make_shared<Animal>(m_genericDeer));
    m_deer[m_deer.size() - 1]->getEntityPointer()->setPos(pos);
    entities.push_back(m_deer[m_deer.size() - 1]->getEntityPointer());
}

void Spirit::spawnWolf(glm::vec3&& pos, std::vector<Entity*>& entities) {
    m_wolves.push_back(std::make_shared<Animal>(m_genericWolf));
    m_wolves[m_wolves.size() - 1]->getEntityPointer()->setPos(pos);
    entities.push_back(m_wolves[m_wolves.size() - 1]->getEntityPointer());
}

void Spirit::spawn(std::vector<Entity*>& entities) {
    m_health = 1;
    m_alive = true;
    bindPlayer(entities);
    int numOfWolves = rand() % 4 + 4;
    for(int i = 0; i < numOfWolves; ++i) {
        spawnWolf(glm::vec3(rand() % BoundingBox::SIZE + m_boundingBox->getEntity()->getPos().x - (BoundingBox::SIZE / 2) , 100, rand() % BoundingBox::SIZE + m_boundingBox->getEntity()->getPos().z - (BoundingBox::SIZE / 2)), entities);
    }
    int numOfDeer = rand() % 4 + 4;
    for(int i = 0; i < numOfDeer; ++i) {
        spawnDeer(glm::vec3(rand() % BoundingBox::SIZE + m_boundingBox->getEntity()->getPos().x - (BoundingBox::SIZE / 2), 100, rand() % BoundingBox::SIZE + m_boundingBox->getEntity()->getPos().z - (BoundingBox::SIZE / 2)), entities);
    }
    entities.push_back(&m_entity);
}

bool Spirit::isAlive() {
    return m_alive;
}

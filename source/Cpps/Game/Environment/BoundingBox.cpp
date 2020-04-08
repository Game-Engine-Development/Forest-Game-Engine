#include "Headers/Game/Environment/BoundingBox.h"

BoundingBox::BoundingBox(Entity *box) : m_entity(box) {
    resetSize();
}

void BoundingBox::resetSize() {
    float scaleOffset = SIZE - m_entity->getScale().x;
    m_entity->addScale(scaleOffset, scaleOffset, scaleOffset);
}

void BoundingBox::shrink() {
    if(m_entity->getScale().x > 50) {
        m_entity->addScale(SPEED, SPEED, SPEED);
    }
}

void BoundingBox::turnOff(std::vector<Entity *> &entities) {
    for(unsigned int i = 0; i < entities.size(); ++i) {
        if(entities[i] == m_entity) {
            entities.erase(entities.begin() + i);
        }
    }
}

void BoundingBox::turnOn(std::vector<Entity *> &entities) {
    resetSize();
    entities.push_back(m_entity);
}

Entity* BoundingBox::getEntity() {
    return m_entity;
}
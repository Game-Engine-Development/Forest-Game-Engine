//
// Created by Alan on 3/17/2020.
//

#include "Headers/Game/Environment/Item.h"

Item::Item(Entity* entity, std::function<void()> action, std::vector<Entity*>& entities) : m_entity{entity}, m_action{action} {
    entities.push_back(m_entity);
}

void Item::update(std::vector<Entity*>& entities, Player* player) {
    if(m_entity->pickedUp && getDistanceToPlayer(player) < 20) {
        pickUp(entities);
        m_entity->pickedUp = false;
    }
}

void Item::pickUp(std::vector<Entity*>& entities) {
    for(auto i = 0; i < entities.size(); ++i) {
        if(entities[i] == m_entity) {
            entities.erase(entities.begin() + i);
        }
    }
    m_action();
}

Entity* Item::getEntity() {
    return m_entity;
}

double Item::getDistanceToPlayer(Player* player) {
    glm::vec3 diff = player->getPlayerEntity().getPos() - m_entity->getPos();
    return std::abs(std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z));
}
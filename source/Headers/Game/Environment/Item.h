#pragma once
#include <functional>
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Game/Player/Player.h"

class Item {
public:
    Item(Entity* entity, std::function<void()> action, std::vector<Entity*>& entities);
    void update(std::vector<Entity*>& entities, Player* player);
    void pickUp(std::vector<Entity*>& entities);
    Entity* getEntity();
private:
    Entity* m_entity;
    std::function<void()> m_action;
    double getDistanceToPlayer(Player* player);
};
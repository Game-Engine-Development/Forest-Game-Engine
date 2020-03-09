#pragma once

#include <Headers/Engine/Models/Entity.h>
#include <Headers/Game/Entities/Spirit.h>

class BoundingBox {
public:
    BoundingBox(Entity* box, Spirit* spirit);
    void shrink();
    void resetSize();
    void turnOn(std::vector<Entity*>& entities);
    void turnOff(std::vector<Entity*>& entities);
    Entity* getEntity();
private:
    Entity* m_entity;
    Spirit* m_spirit;
    float SIZE = 500;
    float const SPEED = -0.2;
};

#pragma once

#include <Headers/Engine/Models/Entity.h>

class BoundingBox {
public:
    BoundingBox(Entity* box);
    void shrink();
    void resetSize();
    void turnOn(std::vector<Entity*>& entities);
    void turnOff(std::vector<Entity*>& entities);
    Entity* getEntity();
private:
    Entity* m_entity;
    float SIZE = 500;
    float const SPEED = -0.2;
};

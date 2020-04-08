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
    static constexpr int SIZE = 500;
    static constexpr float SPEED = -0.5;
private:
    Entity* m_entity;
};

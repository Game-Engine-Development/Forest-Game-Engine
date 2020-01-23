#pragma once

#include <Headers/Engine/Models/Entity.h>
#include <Headers/Engine/Collisions/CollisionHandler.h>

class Wolf {
public:
    Wolf(Entity& entity, CollisionHandler& collisionHandler);

private:
    int health;
    int damage;
    float moveSpeed;
    float jumpHeight;
    Entity m_entity;
    CollisionHandler m_collisionHandler;
};

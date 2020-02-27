#pragma once

#include <Headers/Engine/Models/Entity.h>
#include <Headers/Engine/Collisions/CollisionHandler.h>
#include <Headers/Game/Player/Player.h>

class Wolf {
public:
    Wolf(Entity& entity, Player* player);
    Wolf(Entity&& entity, Player* player);

    void update(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains);
    Entity getEntity();
    Entity* getEntityPointer();
    void render(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor);
    void followPlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains);
    void hitPlayer();
    void takeDamage(int damage);

private:
    bool m_damagedPlayer = false;
    int m_health = 3;
    int m_damage = 5;
    float const MOVE_SPEED = 2.0f;
    float const JUMP_POWER = 1.0f;
    Entity m_entity;
    CollisionHandler m_collisionHandler;
    Player* m_player;

};

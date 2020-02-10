#pragma once

#include <Headers/Engine/Models/Entity.h>
#include <Headers/Engine/Collisions/CollisionHandler.h>
#include <Headers/Game/Player/Player.h>

class Deer {
public:
    Deer(Entity& entity, Player* player);
    Deer(Entity&& entity, Player* player);

    void update(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor, std::vector<Entity*> &entities, std::vector<Terrain*> &terrains);
    Entity getEntity();
    Entity* getEntityPointer();
    void render(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor);
    void followPlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains);
    void hitPlayer();
    void takeDamage(int damage);

private:
    bool m_damagedPlayer = false;
    int m_health = 2;
    int m_damage = 1;
    float const MOVE_SPEED = 1.5f;
    float const JUMP_POWER = 1.0f;
    Entity m_entity;
    CollisionHandler m_collisionHandler;
    Player* m_player;

};

#pragma once

#include <Headers/Game/Environment/BoundingBox.h>
#include <Headers/Game/Entities/Deer.h>
#include <Headers/Game/Entities/Wolf.h>
#include <memory>

class Spirit {
public:
    Spirit(Entity& entity, Player* player, BoundingBox* boundingBox, Wolf& genericWolf, Deer& genericDeer);

    void update(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains);
    Entity getEntity();
    Entity* getEntityPointer();
    void followPlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains);
    void hitPlayer();
    void takeDamage(int damage);
    void updateAnimals(std::vector<Entity *> &entities, std::vector<Terrain *> &terrains);
    bool isBound();
    void spawn(std::vector<Entity*>& entities);
    bool isAlive();
private:
    Wolf m_genericWolf;
    Deer m_genericDeer;
    bool m_damagedPlayer = false;
    bool m_playerBound = false;
    bool m_alive;
    int m_health = 1;
    int m_damage = 20;
    int m_numOfAnimals = 0;
    float const MOVE_SPEED = 1.0f;
    float const JUMP_POWER = 1.0f;
    Entity m_entity;
    CollisionHandler m_collisionHandler;
    Player* m_player;
    BoundingBox* m_boundingBox;
    std::vector<std::shared_ptr<Wolf>> m_wolves;
    std::vector<std::shared_ptr<Deer>> m_deer;
    void spawnDeer(glm::vec3&& pos, std::vector<Entity*>& entities);
    void spawnWolf(glm::vec3&& pos, std::vector<Entity*>& entities);
    void bindPlayer(std::vector<Entity*>& entities);
    void releasePlayer(std::vector<Entity*>& entities);
};

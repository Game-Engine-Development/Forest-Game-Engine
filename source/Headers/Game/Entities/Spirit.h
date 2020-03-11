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
    void render(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor);
    void followPlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains);
    void hitPlayer();
    void takeDamage(int damage);
    void updateAnimals(std::vector<Entity *> &entities, std::vector<Terrain *> &terrains);
    bool isBound();
    void spawnDeer(glm::vec3 pos, std::vector<Entity*>& entities);
    void spawnWolf(glm::vec3 pos, std::vector<Entity*>& entities);
    void bindPlayer(std::vector<Entity*>& entities);
    void releasePlayer(std::vector<Entity*>& entities);
private:
    Wolf m_genericWolf;
    Deer m_genericDeer;
    bool m_damagedPlayer = false;
    bool m_playerBound = false;
    int m_health = 1;
    int m_damage = 20;
    int m_numOfAnimals = 16;
    float const MOVE_SPEED = 1.0f;
    float const JUMP_POWER = 1.0f;
    Entity m_entity;
    CollisionHandler m_collisionHandler;
    Player* m_player;
    BoundingBox* m_boundingBox;
    std::vector<std::shared_ptr<Wolf>> wolves;
    std::vector<std::shared_ptr<Deer>> deer;
};

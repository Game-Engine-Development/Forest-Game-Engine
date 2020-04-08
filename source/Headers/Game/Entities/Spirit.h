#pragma once

#include <array>
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Collisions/CollisionHandler.h"
#include "Headers/Game/Player/Player.h"
#include "Animal.h"
#include <Headers/Game/Environment/BoundingBox.h>
#include <memory>

class Spirit {
public:
    Spirit(Entity& entity, Player* player, BoundingBox* boundingBox, Animal& genericWolf, Animal& genericDeer);

    template <size_t N>
    void update(std::vector<Entity *> &entities, std::array<Terrain, N> &terrains) {
        m_numOfAnimals = m_wolves.size() + m_deer.size();
        if(m_health > 0) {
            if (m_entity.hit) {
                takeDamage(1);
                m_entity.hit = false;
                if(m_health <= 0) {
                    m_alive = false;
                    releasePlayer(entities);
                    for(int i = 0; i < entities.size(); ++i) {
                        if(&m_entity == entities[i]) {
                            entities.erase(entities.begin() + i);
                        }
                    }
                }
            }
            followPlayer(entities, terrains);
            if(m_playerBound) {
                m_boundingBox->shrink();
            }
        }
    }

    template <size_t N>
    void followPlayer(std::vector<Entity*> &entities, std::array<Terrain, N> &terrains) {
        glm::vec3 move = m_player->getPlayerEntity().getPos() - m_entity.getPos();
        if(!m_damagedPlayer) {
            if (std::sqrt(move.x * move.x + move.y * move.y + move.z * move.z) < 20 && !m_collisionHandler.inAir) {
                m_collisionHandler.currentGravity.y = JUMP_POWER;
                m_collisionHandler.inAir = true;
            }
            move.y = 0;
            move = glm::normalize(move);
            move *= MOVE_SPEED;
            m_collisionHandler.moveEntity(move, entities, terrains, m_boundingBox->getEntity(), m_playerBound);
            if(m_collisionHandler.hitPlayer) {
                m_damagedPlayer = true;
                hitPlayer();
            }
        } else if(std::sqrt(move.x * move.x + move.y * move.y + move.z * move.z) > 30) {
            m_damagedPlayer = false;
        }
    }

    template <size_t N>
    void updateAnimals(std::vector<Entity *> &entities, std::array<Terrain, N> &terrains) {
        for(auto i = 0; i < m_wolves.size(); ++i) {
            m_wolves[i]->update(entities, terrains, m_boundingBox->getEntity(), m_playerBound);
            if(m_wolves[i]->isDead()) {
                m_wolves.erase(m_wolves.begin() + i);
            }
        }
        for(auto i = 0; i < m_deer.size(); ++i) {
            m_deer[i]->update(entities, terrains, m_boundingBox->getEntity(), m_playerBound);
            if(m_deer[i]->isDead()) {
                m_deer.erase(m_deer.begin() + i);
            }
        }
    }

    Entity getEntity();
    Entity* getEntityPointer();
    void hitPlayer();
    void takeDamage(int damage);
    bool isBound();
    void spawn(std::vector<Entity*>& entities);
    bool isAlive();
private:
    Animal m_genericWolf;
    Animal m_genericDeer;
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
    std::vector<std::shared_ptr<Animal>> m_wolves;
    std::vector<std::shared_ptr<Animal>> m_deer;
    void spawnDeer(glm::vec3&& pos, std::vector<Entity*>& entities);
    void spawnWolf(glm::vec3&& pos, std::vector<Entity*>& entities);
    void bindPlayer(std::vector<Entity*>& entities);
    void releasePlayer(std::vector<Entity*>& entities);
};

#pragma once

#include <array>
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Collisions/CollisionHandler.h"
#include "Headers/Game/Player/Player.h"
#include "Headers/Game/Entities/Spirit.h"

class Animal {
public:
    Animal();
    Animal(Entity& entity, Player* player, Spirit* spirit, float move_speed, float jump_power);
    Animal(Entity&& entity, Player* player, Spirit* spirit, float move_speed, float jump_power);
    void die();

    void create(Entity& entity, Player* player, Spirit* spirit, float move_speed, float jump_power);
    void create(Entity&& entity, Player* player, Spirit* spirit, float move_speed, float jump_power);

    template<unsigned long N>
    void update(std::vector<Entity *> &entities, std::array<Terrain, N> &terrains) {
        if(m_health > 0) {
            if (m_entity.hit) {
                takeDamage(1);
                m_entity.hit = false;
                if(m_health <= 0) {
                    die();
                    for(int i = 0; i < entities.size(); ++i) {
                        if(&m_entity == entities[i]) {
                            entities.erase(entities.begin() + i);
                        }
                    }
                }
            }
            followPlayer<terrains.size()>(entities, terrains);
        }
    }

    Entity getEntity();
    Entity* getEntityPointer();
    void render(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor);

    template<unsigned long N>
    void followPlayer(std::vector<Entity*> &entities, std::array<Terrain, N> &terrains) {
        glm::vec3 move = m_player->getPlayerEntity().getPos() - m_entity.getPos();
        if(!m_damagedPlayer) {
            if (std::sqrt(move.x * move.x + move.y * move.y + move.z * move.z) < 20 && !m_collisionHandler.inAir) {
                m_collisionHandler.currentGravity.y = m_jump_power;
                m_collisionHandler.inAir = true;
            }
            move.y = 0;
            move = glm::normalize(move);
            move *= m_move_speed;
            m_collisionHandler.moveEntity<terrains.size()>(move, entities, terrains);
            if(m_collisionHandler.hitPlayer) {
                m_damagedPlayer = true;
                hitPlayer();
            }
        } else if(std::sqrt(move.x * move.x + move.y * move.y + move.z * move.z) > 30) {
            m_damagedPlayer = false;
        }
    }

    void hitPlayer();
    void takeDamage(int damage);

private:
    bool m_damagedPlayer = false;
    int m_health = 6;
    int m_damage = 3;
    float m_move_speed = 1.5f;
    float m_jump_power = 1.0f;
    Entity m_entity;
    CollisionHandler m_collisionHandler;
    Player* m_player;
    Spirit* m_spirit;
};

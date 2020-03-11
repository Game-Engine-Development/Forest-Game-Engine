#include "Headers/Game/Entities/Spirit.h"

Spirit::Spirit(Entity &entity, Player* player, BoundingBox* boundingBox, Wolf& genericWolf, Deer& genericDeer) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player), m_boundingBox(boundingBox), m_genericWolf(genericWolf), m_genericDeer(genericDeer) {

}

void Spirit::update(std::vector<Entity *> &entities, std::vector<Terrain *> &terrains) {
    m_numOfAnimals = wolves.size() + deer.size();
    if(m_health > 0) {
        if (m_entity.hit) {
            takeDamage(1);
            m_entity.hit = false;
            if(m_health <= 0) {
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

Entity Spirit::getEntity() {
    return m_entity;
}

Entity* Spirit::getEntityPointer() {
    return &m_entity;
}

void Spirit::render(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor) {
    m_entity.render(camera, shader, lightPos, lightColor);
}

void Spirit::followPlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains) {
    glm::vec3 move = m_player->getPlayerEntity().getPos() - m_entity.getPos();
    if(!m_damagedPlayer) {
        if (std::sqrt(move.x * move.x + move.y * move.y + move.z * move.z) < 20 && !m_collisionHandler.inAir) {
            m_collisionHandler.currentGravity.y = JUMP_POWER;
            m_collisionHandler.inAir = true;
        }
        move.y = 0;
        move = glm::normalize(move);
        move *= MOVE_SPEED;
        m_collisionHandler.moveEntity(move, entities, terrains);
        if(m_collisionHandler.hitPlayer) {
            m_damagedPlayer = true;
            hitPlayer();
        }
    } else if(std::sqrt(move.x * move.x + move.y * move.y + move.z * move.z) > 30) {
        m_damagedPlayer = false;
    }
}

void Spirit::hitPlayer() {
    m_player->takeDamage(m_damage);
}

void Spirit::takeDamage(int damage) {
    if(m_numOfAnimals <= 0) {
        m_health -= damage;
    }
}

bool Spirit::isBound() {
    return m_playerBound;
}

void Spirit::bindPlayer(std::vector<Entity*>& entities) {
    m_playerBound = true;
    m_boundingBox->turnOn(entities);
}

void Spirit::releasePlayer(std::vector<Entity *> &entities) {
    m_playerBound = false;
    m_boundingBox->turnOff(entities);
}

void Spirit::spawnDeer(glm::vec3 pos, std::vector<Entity*>& entities) {
    std::shared_ptr<Deer> newDeer = std::make_shared<Deer>(m_genericDeer);
    newDeer->getEntity().setPos(pos);
    deer.push_back(newDeer);
    entities.push_back(newDeer->getEntityPointer());
}

void Spirit::spawnWolf(glm::vec3 pos, std::vector<Entity*>& entities) {
    std::shared_ptr<Wolf> newWolf = std::make_shared<Wolf>(m_genericWolf);
    newWolf->getEntity().setPos(pos);
    wolves.push_back(newWolf);
    entities.push_back(newWolf->getEntityPointer());
}

void Spirit::updateAnimals(std::vector<Entity *> &entities, std::vector<Terrain *> &terrains) {
    for(unsigned int i = 0; i < wolves.size(); ++i) {
        wolves[i]->update(entities, terrains);
        if(wolves[i]->isDead()) {
            wolves.erase(wolves.begin() + i);
        }
    }
    for(unsigned int i = 0; i < deer.size(); ++i) {
        deer[i]->update(entities, terrains);
        if(deer[i]->isDead()) {
            deer.erase(deer.begin() + i);
        }
    }
}

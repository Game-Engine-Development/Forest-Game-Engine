#pragma once

#include <array>
#include <algorithm>
#include <utility>

#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Terrain/Terrain.h"



bool checkPointInTriangle(glm::vec3 point, glm::vec3 pa, glm::vec3 pb, glm::vec3 pc);

std::pair<bool, float> getLowestRoot(float a, float b, float c, float maxR, float root);

float squareAndSum(glm::vec3 data);



class CollisionHandler {
private:
    struct Movement {
        glm::vec3 eRadius{};
        glm::vec3 movement{};
        glm::vec3 startingPos{};
        glm::vec3 eSpaceMovement{};
        glm::vec3 eSpaceMovementNormalized{};
        glm::vec3 eSpaceStartingPos{};

        bool foundCollision{};
        double nearestDistance{};
        glm::vec3 intersectionPoint{};
    };

    Entity* m_entity;
    Terrain* currentTerrain{};
    int collisionRecursionDepth = 0;

    Entity* m_hitEntity = nullptr;

    template <size_t N>
    Terrain* calculateCurrentTerrain(std::array<Terrain, N> &terrains);

    std::vector<Plane> calculateCollidablePlanes(std::vector<Plane>& planes);

    void checkTriangle(const Plane &trianglePlane);
    void collideAndSlide(
            glm::vec3 vel,
            glm::vec3 gravity,
            std::vector<Entity>& entities
    );
    glm::vec3 collideWithWorld(
            glm::vec3 pos,
            glm::vec3 vel,
            std::vector<Entity>& entities
    );
    void calculateCollisions(const std::vector<Plane>& planes);
    void calculateTerrainCollisions(glm::vec3 &finalMove);
    void updateGravity();
    float simGravity(float tics) const;

public:
    static constexpr glm::vec3 GRAVITY = glm::vec3(0, -0.08f, 0);
    static constexpr float unitsPerMeter = 100;

    bool inAir = false;
    glm::vec3 currentGravity = glm::vec3(0,0,0);
    Movement move{};

    explicit CollisionHandler(Entity* entity);

    template<size_t N>
    void moveEntity(glm::vec3 &finalMove, std::vector<Entity> &entities, std::array<Terrain, N> &terrains);

    template <size_t N>
    void moveEntityWithoutGravity(glm::vec3 &finalMove, std::vector<Entity> &entities, std::array<Terrain, N> &terrains);
};

//definitions of template functions:

template <size_t N>
Terrain* CollisionHandler::calculateCurrentTerrain(std::array<Terrain, N> &terrains) {
    for(Terrain& terrain : terrains) {
        if(
                m_entity->getPos().x >= terrain.getPos().x
                && m_entity->getPos().x <= terrain.getPos().x + TerrainMesh::SIZE
                && m_entity->getPos().z >= terrain.getPos().z
                && m_entity->getPos().z <= terrain.getPos().z + TerrainMesh::SIZE)
        {
            return &terrain;
        }
    }
    return &terrains[0];
}

template<size_t N>
void CollisionHandler::moveEntity(
        glm::vec3 &finalMove,
        std::vector<Entity> &entities,
        std::array<Terrain, N> &terrains)
{
    currentTerrain = calculateCurrentTerrain(terrains);
    collideAndSlide(finalMove, currentGravity, entities);
    updateGravity();
}

template <size_t N>
void CollisionHandler::moveEntityWithoutGravity(
        glm::vec3 &finalMove,
        std::vector<Entity> &entities,
        std::array<Terrain, N> &terrains)
{
    currentTerrain = calculateCurrentTerrain(terrains);

    calculateTerrainCollisions(finalMove);
    currentGravity = glm::vec3(0);

    collideAndSlide(finalMove, currentGravity, entities);
    updateGravity();
}

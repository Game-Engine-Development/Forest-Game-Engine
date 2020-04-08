#pragma once

#include <array>
#include <Headers/Engine/Models/Entity.h>
#include <Headers/Engine/Terrain/Terrain.h>

struct Movement {
    Movement() = default;
    glm::vec3 eRadius;
    glm::vec3 movement;
    glm::vec3 startingPos;
    glm::vec3 eSpaceMovement;
    glm::vec3 eSpaceMovementNormalized;
    glm::vec3 eSpaceStartingPos;

    bool foundCollision;
    bool hitPlayer;
    double nearestDistance;
    glm::vec3 intersectionPoint;
};

class CollisionHandler {
private:
    Entity* m_entity;
    Terrain* currentTerrain;
    int collisionRecursionDepth = 0;

    Entity* m_hitEntity = nullptr;

    template <unsigned long N>
    Terrain* calculateCurrentTerrain(std::array<Terrain, N> &terrains) {
        for(Terrain& terrain : terrains) {
            if(
                    m_entity->getPos().x >= terrain.getPos().x
                    && m_entity->getPos().x <= terrain.getPos().x + TerrainMesh::SIZE
                    && m_entity->getPos().z >= terrain.getPos().z
                    && m_entity->getPos().z <= terrain.getPos().z + TerrainMesh::SIZE
                ) {
                return &terrain;
            }
        }
        return &terrains[0];
    }

    std::vector<Plane> calculateCollidablePlanes(std::vector<Plane>& planes);
    static bool getLowestRoot(float a, float b, float c, float maxR, float* root);
    static bool checkPointInTriangle(
            const glm::vec3& point,
            const glm::vec3& pa,
            const glm::vec3& pb,
            const glm::vec3& pc
    );
    void checkTriangle(const Plane &trianglePlane, Entity* entity);
    void collideAndSlide(
            const glm::vec3& vel,
            const glm::vec3& gravity,
            std::vector<Entity*>& entities
    );
    glm::vec3 collideWithWorld(
            const glm::vec3& pos,
            const glm::vec3& vel,
            std::vector<Entity*>& entities
    );
    void calculateCollisions(std::vector<Plane>& planes, Entity* entity);
    void calculateTerrainCollisions(glm::vec3 &finalMove);
    void updateGravity();
    float simGravity(float tics);
public:
    static constexpr glm::vec3 GRAVITY = glm::vec3(0, -0.08f, 0);
    static constexpr float unitsPerMeter = 100;

    bool inAir = false;
    bool hitPlayer = false;
    glm::vec3 currentGravity = glm::vec3(0,0,0);
    Movement move;

    CollisionHandler();
    explicit CollisionHandler(Entity* entity);

    template<unsigned long N>
    void moveEntity(
            glm::vec3 &finalMove,
            std::vector<Entity*>& entities,
            std::array<Terrain, N>& terrains
        ) {
        hitPlayer = false;
        currentTerrain = calculateCurrentTerrain(terrains);
        calculateTerrainCollisions(finalMove);
        collideAndSlide(finalMove, currentGravity, entities);
        updateGravity();
    }

    template <unsigned long N>
    void moveEntityWithoutGravity(
            glm::vec3 &finalMove,
            std::vector<Entity*>& entities,
            std::array<Terrain, N>& terrains
        ) {
        hitPlayer = false;
        currentTerrain = calculateCurrentTerrain(terrains);
        calculateTerrainCollisions(finalMove);
        currentGravity = glm::vec3(0, 0, 0);
        collideAndSlide(finalMove, currentGravity, entities);
        updateGravity();
    }
};

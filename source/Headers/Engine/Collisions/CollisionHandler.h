#pragma once

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

    Terrain* calculateCurrentTerrain(std::vector<Terrain*>& terrains);
    std::vector<Plane> calculateCollidablePlanes(std::vector<Plane>& planes);
    bool getLowestRoot(float a, float b, float c, float maxR, float* root);
    bool checkPointInTriangle(const glm::vec3& point, const glm::vec3& pa,const glm::vec3& pb, const glm::vec3& pc);
    void checkTriangle(const Plane &trianglePlane, Entity* entity);
    void collideAndSlide(const glm::vec3& vel, const glm::vec3& gravity, std::vector<Entity*>& entities);
    glm::vec3 collideWithWorld(const glm::vec3& pos, const glm::vec3& vel, std::vector<Entity*>& entities);
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
    void moveEntity(glm::vec3 &finalMove, std::vector<Entity*>& entities, std::vector<Terrain*>& terrains);
    void moveEntityWithoutGravity(glm::vec3 &finalMove, std::vector<Entity*>& entities, std::vector<Terrain*>& terrains);
};

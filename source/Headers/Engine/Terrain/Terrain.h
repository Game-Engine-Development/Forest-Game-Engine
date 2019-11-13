#pragma once

#include <Headers/Engine/Camera.h>
#include "Headers/Engine/Terrain/TerrainTextureMap.h"
#include "Headers/Engine/Terrain/TerrainMesh.h"
#include "Headers/Engine/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Terrain {

public:
    Terrain(TerrainTextureMap& textureMap, TerrainMesh& terrainMesh, int gridX, int gridY);
    ~Terrain();
    void render(Camera& camera, Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor);
    float getTerrainHeight(float worldX, float worldZ);
    glm::vec3 getTerrainNormal(float worldX, float worldZ);
    glm::vec3 getPos();
    TerrainMesh getTerrainMesh();
private:
    TerrainTextureMap terrainTextureMap;
    TerrainMesh terrainMesh;
    int x, z;
    glm::vec3 position;
    glm::mat4 modelMatrix;
    glm::mat4 createModelMatrix();
    float getHeight(int vertexX, int vertexZ);
    float getAverageHeight(float terrainX, float terrainZ);
    float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
};


#pragma once

#include <Headers/Engine/Camera.h>
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Terrain/TerrainMesh.h"
#include "Headers/Engine/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Terrain {

public:
    Terrain(Texture& texture, TerrainMesh& terrainMesh, int gridX, int gridY);
    ~Terrain();
    void render(Camera& camera, Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor);
    float getTerrainHeight(float worldX, float worldZ);
    glm::vec3 getPos();
private:
    Texture terrainTexture;
    TerrainMesh terrainMesh;
    int x, z;
    glm::vec3 position;
    glm::mat4 modelMatrix;
    glm::mat4 createModelMatrix();
    float getHeight(float playerX, float playerZ);
};


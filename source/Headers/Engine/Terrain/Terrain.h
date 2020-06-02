#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Terrain/TerrainTextureMap.h"
#include "Headers/Engine/Terrain/TerrainMesh.h"
#include "Headers/Engine/Shader/Shader.h"
#include <Headers/Engine/Graphics/Lighting/PointLight.h>
#include <Headers/Engine/Graphics/Materials/Material.h>



class Terrain {
public:
    Terrain();

    Terrain(TerrainTextureMap textureMap, TerrainMesh &terrainMesh, int gridX, int gridY);

    void create(TerrainTextureMap textureMap, TerrainMesh &mesh, int gridX, int gridZ);

    void render(Camera& camera, Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor);

    void render(Camera &camera, Shader &shader, std::vector<PointLight> pointLights, Material material);

    float getTerrainHeight(float worldX, float worldZ);
    glm::vec3 getTerrainNormal(float worldX, float worldZ);
    glm::vec3 getPos();
    TerrainMesh &getTerrainMesh();
    ~Terrain();
private:
    TerrainTextureMap terrainTextureMap;
    TerrainMesh terrainMesh;
    glm::vec3 position{};
    glm::mat4 modelMatrix{};
    glm::mat4 createModelMatrix();
    float getHeight(int vertexX, int vertexZ);
    float getAverageHeight(float terrainX, float terrainZ);
    static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
};


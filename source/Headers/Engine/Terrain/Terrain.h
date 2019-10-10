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
    Terrain(Texture& texture, int gridX, int gridY);
    ~Terrain();
    void render(Camera camera, Shader& shader);
private:
    const float SIZE = 800;
    const int VERTEX_COUNT = 128;
    Texture texture;
    TerrainMesh terrainMesh;
    int x, z;
    glm::vec3 position;
    glm::mat4 modelMatrix;
    glm::mat4 createModelMatrix();
};


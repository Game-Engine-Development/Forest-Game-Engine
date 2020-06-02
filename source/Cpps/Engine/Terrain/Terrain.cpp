#include "Headers/Engine/Terrain/Terrain.h"

Terrain::Terrain() = default;

Terrain::Terrain(TerrainTextureMap textureMap, TerrainMesh &mesh, int gridX, int gridZ) {
    float x = (float)gridX * TerrainMesh::SIZE;
    float z = (float)gridZ * TerrainMesh::SIZE;
    terrainMesh = mesh;
    terrainTextureMap = std::move(textureMap);
    position = glm::vec3(x, 0, z);
}

Terrain::~Terrain() = default;

void Terrain::render(Camera &camera, Shader &shader, glm::vec3& lightPos, glm::vec3& lightColor) {
    shader.use();
    terrainMesh.bindVAO();
    terrainTextureMap.bindTextures(shader);
    camera.setMatrices(shader);
    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(createModelMatrix()));
    int lightPosLoc = glGetUniformLocation(shader.ID, "lightPos");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    int lightColorLoc = glGetUniformLocation(shader.ID, "lightColor");
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    int viewPosLoc = glGetUniformLocation(shader.ID, "viewPos");
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.getPos()));
    glDrawElements(GL_TRIANGLES, terrainMesh.getNumOfVertices(), GL_UNSIGNED_INT, nullptr);
    terrainTextureMap.unBindTextures();
    TerrainMesh::unbindVAO();
}

void Terrain::render(Camera &camera, Shader &shader, std::vector<PointLight> pointLights, Material material) {
    shader.use();

    terrainMesh.bindVAO();

    material.getAlbedo().bind(shader);
    material.getAo().bind(shader);
    material.getMetallic().bind(shader);
    material.getNormal().bind(shader);
    material.getRoughness().bind(shader);

    camera.setMatrices(shader);

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(createModelMatrix()));

    int viewLoc = glGetUniformLocation(shader.ID, "viewPos");
    glUniform3fv(viewLoc, 1, glm::value_ptr(camera.getPos()));

    for(int i = 0; i < pointLights.size(); ++i) {
        shader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
        shader.setVec3("pointLights[" + std::to_string(i) + "].color", pointLights[i].color);
        shader.setFloat("pointLights[" + std::to_string(i) + "].intensity", pointLights[i].intensity);
    }

    shader.setFloat("nLights", pointLights.size());
    //shader.setVec3("test", glm::vec3(1,0,0));

    glDrawElements(GL_TRIANGLES, terrainMesh.getNumOfVertices(), GL_UNSIGNED_INT, nullptr);

    material.getAlbedo().unbind();
    material.getAo().unbind();
    material.getMetallic().unbind();
    material.getNormal().unbind();
    material.getRoughness().unbind();
    TerrainMesh::unbindVAO();
}

glm::vec3 Terrain::getPos(){
    return position;
}

glm::mat4 Terrain::createModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    return modelMatrix;
}

float Terrain::getHeight(int vertexX, int vertexZ){
    return terrainMesh.getHeight((float)vertexX, (float)vertexZ);
}

float Terrain::getTerrainHeight(float worldX, float worldZ) {
    float x = worldX / TerrainMesh::SIZE * terrainMesh.getWidth();
    while(x > terrainMesh.getWidth()) {
        x -= terrainMesh.getWidth();
    }
    while(x < 0) {
        x += terrainMesh.getWidth();
    }
    float z = worldZ / TerrainMesh::SIZE * terrainMesh.getWidth();
    while(z > terrainMesh.getWidth()) {
        z -= terrainMesh.getWidth();
    }
    while(z < 0) {
        z += terrainMesh.getWidth();
    }
    return getAverageHeight(x, z);
}

float Terrain::getAverageHeight(float terrainX, float terrainZ) {
    float bottomLeft = getHeight((int)terrainX, (int)terrainZ);
    float bottomRight = getHeight((int)terrainX, (int)terrainZ + 1);
    float topLeft = getHeight((int)terrainX + 1, (int)terrainZ);
    float topRight = getHeight((int)terrainX + 1, (int)terrainZ + 1);
    float x = terrainX;
    float z = terrainZ;
    while(x > 1) {
        --x;
    }
    while(z > 1) {
        --z;
    }
    float height;
    if (x <= (1-z)) {
        height = barryCentric(glm::vec3(0, bottomLeft, 0), glm::vec3(1, topLeft, 0), glm::vec3(0, bottomRight, 1), glm::vec2(x, z));
    } else {
        height = barryCentric(glm::vec3(1, topLeft, 0), glm::vec3(1, topRight, 1), glm::vec3(0, bottomRight, 1), glm::vec2(x, z));
    }
    return height;
}

glm::vec3 Terrain::getTerrainNormal(float worldX, float worldZ) {
    float x = worldX / TerrainMesh::SIZE * terrainMesh.getWidth();
    while(x > terrainMesh.getWidth()) {
        x -= terrainMesh.getWidth();
    }
    while(x < 0) {
        x += terrainMesh.getWidth();
    }
    float z = worldZ / TerrainMesh::SIZE * terrainMesh.getWidth();
    while(z > terrainMesh.getWidth()) {
        z -= terrainMesh.getWidth();
    }
    while(z < 0) {
        z += terrainMesh.getWidth();
    }
    return terrainMesh.calculateNormal(x, z);
}

float Terrain::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    float l3 = 1.0f - l1 - l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

TerrainMesh &Terrain::getTerrainMesh() {
    return terrainMesh;
}

void Terrain::create(TerrainTextureMap textureMap, TerrainMesh &mesh, int gridX, int gridZ) {
    float x = (float)gridX * TerrainMesh::SIZE;
    float z = (float)gridZ * TerrainMesh::SIZE;
    terrainMesh = mesh;
    terrainTextureMap = std::move(textureMap);
    position = glm::vec3(x, 0, z);
}
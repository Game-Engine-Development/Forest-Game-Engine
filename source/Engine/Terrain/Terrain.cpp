#include "Headers/Engine/Terrain/Terrain.h"

Terrain::Terrain(TerrainTextureMap &textureMap, TerrainMesh &mesh, int gridX, int gridZ) {
    x = gridX * TerrainMesh::SIZE;
    z = gridZ * TerrainMesh::SIZE;
    terrainMesh = mesh;
    terrainTextureMap = textureMap;
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
    glDrawElements(GL_TRIANGLES, terrainMesh.getNumOfVertices(), GL_UNSIGNED_INT, 0);
    terrainTextureMap.unBindTextures();
    terrainMesh.unbindVAO();
}

glm::vec3 Terrain::getPos(){
    return position;
}

glm::mat4 Terrain::createModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    return modelMatrix;
}

float Terrain::getHeight(float playerX, float playerZ){
    return terrainMesh.getHeight(playerX, playerZ);
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
    return getHeight((int)x, (int)z);
}
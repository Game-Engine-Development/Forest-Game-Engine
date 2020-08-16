#include "Headers/Engine/Terrain/Terrain.h"

Terrain::Terrain(std::variant<TerrainTextureMap, Texture> textureMap, TerrainMesh *const mesh, const int gridX, const int gridZ) noexcept {
    const float x = static_cast<float>(gridX) * TerrainMesh::SIZE;
    const float z = static_cast<float>(gridZ) * TerrainMesh::SIZE;
    terrainMesh = mesh;
    terrainTextureMap = std::move(textureMap);
    position = glm::vec3(x, 0, z);
}

void Terrain::render(const Camera& camera, const Shader& shader, glm::vec3 lightPos, glm::vec3 lightColor) {
    shader.use();
    terrainMesh->bindVAO();
    std::visit([&](auto&& arg){arg.bind(shader);}, terrainTextureMap);
    //terrainTextureMap.bind(shader);
    camera.setMatrices(shader);
    const int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(createModelMatrix()));
    const int lightPosLoc = glGetUniformLocation(shader.ID, "lightPos");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    const int lightColorLoc = glGetUniformLocation(shader.ID, "lightColor");
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    const int viewPosLoc = glGetUniformLocation(shader.ID, "viewPos");
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.getPos()));
    glDrawElements(GL_TRIANGLES, terrainMesh->getNumOfVertices(), GL_UNSIGNED_INT, nullptr);
    std::visit([&](auto&& arg){arg.unbind();}, terrainTextureMap);
    //terrainTextureMap.unbind();
    TerrainMesh::unbindVAO();
}

[[nodiscard]] glm::vec3 Terrain::getPos() const noexcept {
    return position;
}

[[nodiscard]] glm::mat4 Terrain::createModelMatrix() const noexcept {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    return modelMatrix;
}

float Terrain::getHeight(int vertexX, int vertexZ) const noexcept {
    return terrainMesh->getHeight((float)vertexX, (float)vertexZ);
}

[[nodiscard]] float Terrain::getTerrainHeight(float worldX, float worldZ) const noexcept {
    float x = worldX / TerrainMesh::SIZE * terrainMesh->getWidth();
    while(x > terrainMesh->getWidth()) {
        x -= terrainMesh->getWidth();
    }
    while(x < 0) {
        x += terrainMesh->getWidth();
    }
    float z = worldZ / TerrainMesh::SIZE * terrainMesh->getWidth();
    while(z > terrainMesh->getWidth()) {
        z -= terrainMesh->getWidth();
    }
    while(z < 0) {
        z += terrainMesh->getWidth();
    }
    return getAverageHeight(x, z);
}

float Terrain::getAverageHeight(float terrainX, float terrainZ) const noexcept {
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

[[nodiscard]] glm::vec3 Terrain::getTerrainNormal(float worldX, float worldZ) const noexcept {
    float x = worldX / TerrainMesh::SIZE * terrainMesh->getWidth();
    while(x > terrainMesh->getWidth()) {
        x -= terrainMesh->getWidth();
    }
    while(x < 0) {
        x += terrainMesh->getWidth();
    }
    float z = worldZ / TerrainMesh::SIZE * terrainMesh->getWidth();
    while(z > terrainMesh->getWidth()) {
        z -= terrainMesh->getWidth();
    }
    while(z < 0) {
        z += terrainMesh->getWidth();
    }
    return terrainMesh->calculateNormal(x, z);
}

float Terrain::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) noexcept {
    const float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    const float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    const float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    const float l3 = 1.0f - l1 - l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

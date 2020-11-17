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
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(createModelMatrixTerrain()));
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

[[nodiscard]] glm::mat4 Terrain::createModelMatrixTerrain() const noexcept {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    return modelMatrix;
}

[[nodiscard]] float Terrain::getHeight(const int vertexX, const int vertexZ) const noexcept {
    return terrainMesh->getHeight(vertexX, vertexZ);
}

[[nodiscard]] float Terrain::getTerrainHeight(const float worldX, const float worldZ) const noexcept {
    const auto getTerrainHeight1D = [](const TerrainMesh *const terrainMesh, const float worldN) {
        const float x = (worldN / TerrainMesh::SIZE) * terrainMesh->getWidth();

        float intX{};
        const float decimal = std::modf(x, &intX);
        const float clampedAbsX = std::abs(static_cast<int>(intX)) % terrainMesh->getWidth();
        return ((x < 0) ? terrainMesh->getWidth() - clampedAbsX : clampedAbsX) + decimal;
    };

    return getAverageHeight(getTerrainHeight1D(terrainMesh, worldX), getTerrainHeight1D(terrainMesh, worldZ));
}

float Terrain::getAverageHeight(const float terrainX, const float terrainZ) const noexcept {
    const auto getDecimal = [](const float value) {
        float intValue{};
        return std::modf(value, &intValue);
    };

    const float bottomLeft = getHeight((int)terrainX, (int)terrainZ);
    const float bottomRight = getHeight((int)terrainX, (int)terrainZ + 1);
    const float topLeft = getHeight((int)terrainX + 1, (int)terrainZ);
    const float topRight = getHeight((int)terrainX + 1, (int)terrainZ + 1);

    const float x = getDecimal(terrainX);
    const float z = getDecimal(terrainZ);
    if (x <= (1-z)) {
        return barryCentric(glm::vec3(0, bottomLeft, 0), glm::vec3(1, topLeft, 0), glm::vec3(0, bottomRight, 1), glm::vec2(x, z));
    }
    return barryCentric(glm::vec3(1, topLeft, 0), glm::vec3(1, topRight, 1), glm::vec3(0, bottomRight, 1), glm::vec2(x, z));
}

[[nodiscard]] glm::vec3 Terrain::getTerrainNormal(const float worldX, const float worldZ) const noexcept {
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

float Terrain::barryCentric(const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3, const glm::vec2 pos) noexcept {
    const float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    const float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    const float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    const float l3 = 1.0f - l1 - l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

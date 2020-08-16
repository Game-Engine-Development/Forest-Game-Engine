#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <variant>

#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Terrain/TerrainTextureMap.h"
#include "Headers/Engine/Terrain/TerrainMesh.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Texture/Texture.h"

class Terrain {
    std::variant<TerrainTextureMap, Texture> terrainTextureMap;
    TerrainMesh *terrainMesh{};
    glm::vec3 position{};

    [[nodiscard]] glm::mat4 createModelMatrix() const noexcept;
    [[nodiscard]] float getHeight(int vertexX, int vertexZ) const noexcept;
    [[nodiscard]] float getAverageHeight(float terrainX, float terrainZ) const noexcept;
    static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) noexcept;

public:
    Terrain() noexcept = default;

    Terrain(std::variant<TerrainTextureMap, Texture> textureMap, TerrainMesh *terrainMesh, int gridX, int gridY) noexcept;

    void render(const Camera& camera, const Shader& shader, glm::vec3 lightPos, glm::vec3 lightColor);

    [[nodiscard]] float getTerrainHeight(float worldX, float worldZ) const noexcept;
    [[nodiscard]] glm::vec3 getTerrainNormal(float worldX, float worldZ) const noexcept;
    [[nodiscard]] glm::vec3 getPos() const noexcept;
};


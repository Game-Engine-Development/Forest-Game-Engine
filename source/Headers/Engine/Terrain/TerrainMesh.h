#pragma once

#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Headers/Engine/ResourceContainers/stb_PointerContainer.h"
#include "Headers/Engine/Utils/NoiseFunctions/OpenSimplex2S.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#include "../../../../libraries/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#endif

class TerrainMesh {
    std::shared_ptr<stb_PointerContainer> dataContainer = nullptr;

    std::optional<Noise::OpenSimplex2S> noise;

    static constexpr float multiplier = 50.f;

    int height{};
    void loadTerrain(std::vector<glm::vec3>& verticies, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<unsigned int>& indices, const char* filename);
    unsigned int VAO{}, VBO{}, EBO{}, texCoordBuffer{}, normalBuffer{}, numOfVertices{};

public:
    TerrainMesh(const TerrainMesh &terrainMeshObj) = delete;
    TerrainMesh(TerrainMesh &&oldTerrainMeshObj) = delete;

    explicit TerrainMesh(const char* filename);
    explicit TerrainMesh(const char* filename, long seed);

    TerrainMesh &operator=(const TerrainMesh &terrainMeshObj) = delete;
    TerrainMesh &operator=(TerrainMesh &&oldTerrainMeshObj) = delete;

    void bindVAO() const;

    static void unbindVAO();

    [[nodiscard]] unsigned int getNumOfVertices() const;

    static constexpr float SIZE = 1600;
    static constexpr float MAX_HEIGHT = 80;

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] float getHeightFromNoise(double x, double z) const;
    [[nodiscard]] float getHeightFromHeightmap(int x, int z) const;
    [[nodiscard]] glm::vec3 calculateNormal(int x, int z) const;

    [[nodiscard]] float getHeight(int x, int z) const;

    ~TerrainMesh();
};
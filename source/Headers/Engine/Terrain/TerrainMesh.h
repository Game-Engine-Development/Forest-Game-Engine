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
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;

    std::unique_ptr<stb_PointerContainer> dataContainer = nullptr;

    std::optional<Noise::OpenSimplex2S> noise;

    static constexpr float multiplier = 50.f;

    //x or z:
    int height{};

    //y:
    float minHeight{};
    float maxHeight{};

    unsigned int VAO{}, VBO{}, EBO{}, texCoordBuffer{}, normalBuffer{}, numOfVertices{};

    void loadTerrain(std::vector<glm::vec3>& verticies, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<unsigned int>& indices, const char* filename);

    friend void swap(TerrainMesh &terrainMesh1, TerrainMesh &terrainMesh2) {
        std::swap(terrainMesh1.vertices, terrainMesh2.vertices);
        std::swap(terrainMesh1.normals, terrainMesh2.normals);
        std::swap(terrainMesh1.texCoords, terrainMesh2.texCoords);
        std::swap(terrainMesh1.indices, terrainMesh2.indices);

        std::swap(terrainMesh1.dataContainer, terrainMesh2.dataContainer);

        std::swap(terrainMesh1.noise, terrainMesh2.noise);

        std::swap(terrainMesh1.height, terrainMesh2.height);

        std::swap(terrainMesh1.VAO, terrainMesh2.VAO);
        std::swap(terrainMesh1.VBO, terrainMesh2.VBO);
        std::swap(terrainMesh1.EBO, terrainMesh2.EBO);
        std::swap(terrainMesh1.texCoordBuffer, terrainMesh2.texCoordBuffer);
        std::swap(terrainMesh1.normalBuffer, terrainMesh2.normalBuffer);

        std::swap(terrainMesh1.numOfVertices, terrainMesh2.numOfVertices);
    }

public:
    TerrainMesh(const TerrainMesh &terrainMeshObj) = delete;
    TerrainMesh(TerrainMesh &&oldTerrainMeshObj) noexcept
    : vertices(std::move(oldTerrainMeshObj.vertices)),
    normals(std::move(oldTerrainMeshObj.normals)),
    texCoords(std::move(oldTerrainMeshObj.texCoords)),
    indices(std::move(oldTerrainMeshObj.indices)),
    dataContainer(std::move(oldTerrainMeshObj.dataContainer)),
    noise(oldTerrainMeshObj.noise),
    height(oldTerrainMeshObj.height),
    VAO(oldTerrainMeshObj.VAO),
    VBO(oldTerrainMeshObj.VBO),
    EBO(oldTerrainMeshObj.EBO),
    texCoordBuffer(oldTerrainMeshObj.texCoordBuffer),
    normalBuffer(oldTerrainMeshObj.normalBuffer),
    numOfVertices(oldTerrainMeshObj.numOfVertices)
    {
        oldTerrainMeshObj.dataContainer = nullptr;

        oldTerrainMeshObj.noise = std::nullopt;

        oldTerrainMeshObj.height = 0;

        oldTerrainMeshObj.VAO = 0;
        oldTerrainMeshObj.VBO = 0;
        oldTerrainMeshObj.EBO = 0;
        oldTerrainMeshObj.texCoordBuffer = 0;
        oldTerrainMeshObj.normalBuffer = 0;

        oldTerrainMeshObj.numOfVertices = 0;
    }

    explicit TerrainMesh(const char* filename);
    explicit TerrainMesh(const char* filename, long seed);

    TerrainMesh &operator=(const TerrainMesh &terrainMeshObj) = delete;
    TerrainMesh &operator=(TerrainMesh &&oldTerrainMeshObj) noexcept {
        TerrainMesh move(std::move(oldTerrainMeshObj));
        swap(*this, move);
        return *this;
    }

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

    [[nodiscard]] float getMinHeight() const;
    [[nodiscard]] float getMaxHeight() const;

    [[nodiscard]] glm::vec2 getX_Y_InWorldCoords(float indexX, float indexY) const noexcept;

    ~TerrainMesh();
};
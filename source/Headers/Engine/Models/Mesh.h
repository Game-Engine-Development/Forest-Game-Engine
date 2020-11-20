#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <optional>
#include <mutex>
#include <future>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Headers/Engine/ResourceContainers/MeshResourceContainer.h"

class Mesh {
    friend void swap(Mesh &mesh1, Mesh &mesh2);

    struct FilePointerWrapper;

    static inline std::unordered_map<std::string, std::pair<std::optional<MeshResourceContainer>, std::size_t>> meshCache;
    std::string meshCacheKey;

    unsigned int VAOCache{};
    std::size_t numOfVerticesCache{};

    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> textureCoords;
    bool normalMapped;
    glm::vec3 greatest{}, smallest{};

    void loadOBJ(
            const char *filename,
            std::vector<glm::vec3> &vertices,
            std::vector<glm::vec2> &textureCoords,
            std::vector<glm::vec3> &normals
    );
    static void CalculateTangentsAndBitangents(
            std::vector<glm::vec3> &vertices,
            std::vector<glm::vec2> &uvs,
            std::vector<glm::vec3> &normals,
            std::vector<glm::vec3> &tangents,
            std::vector<glm::vec3> &bitangents
    );
    void updateCube(glm::vec3 &vertex) const;

public:
    explicit Mesh(const std::string &filename, bool isNormalMapped);

    Mesh(const Mesh &mesh);
    Mesh(Mesh &&oldMesh) noexcept;

    Mesh& operator=(const Mesh &mesh);
    Mesh& operator=(Mesh &&oldMesh) noexcept;

    void bindVAO() const;
    static void unbindVAO();

    [[nodiscard]] unsigned int getNumOfVertices() const;
    [[nodiscard]] const std::vector<glm::vec3>& getVertices() const noexcept;

    ~Mesh();
};



struct Mesh::FilePointerWrapper {
    FilePointerWrapper(const FilePointerWrapper&) = delete;
    FilePointerWrapper& operator=(const FilePointerWrapper&) = delete;
    FilePointerWrapper(FilePointerWrapper&&) = delete;
    FilePointerWrapper& operator=(FilePointerWrapper&&) = delete;

    FilePointerWrapper(const char *filename, const char *modes);

    [[nodiscard]] FILE* getFile() const noexcept;

    ~FilePointerWrapper();

private:
    FILE *const file;
};

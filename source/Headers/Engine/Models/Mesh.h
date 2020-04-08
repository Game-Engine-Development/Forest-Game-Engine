#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Mesh {
public:
    Mesh();
    Mesh(Mesh& mesh);
    explicit Mesh(const char* filename, bool isNormalMapped);
    ~Mesh();
    void bindVAO();
    static void unbindVAO();
    unsigned int getNumOfVertices();
    std::vector<glm::vec3>& getVertices();
    std::vector<glm::vec3>& getNormals();
    std::vector<glm::vec2>& getTextureCoords();
    bool isNormalMapped();
    std::vector<glm::vec3>& getCollisionCube();
    double getVerticalOffset();
private:
    unsigned int VAO, VBO, texCoordBuffer, normalBuffer, tangentBuffer, bitangentBuffer, numOfVertices;
    std::vector<glm::vec3> vertices, normals, collisionCube;
    std::vector<glm::vec2> textureCoords;
    bool normalMapped;
    double greatestX = 0, greatestY = 0, greatestZ = 0, smallestX = 0, smallestY = 0, smallestZ = 0;

    void loadOBJ(const char* filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& textureCoords, std::vector<glm::vec3>& normals);
    void CalculateTangentsAndBitangents(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals,
                      std::vector<glm::vec3> &tangents, std::vector<glm::vec3> &bitangents);
    void updateCube(glm::vec3& vertex);
    void calculateCube();
};


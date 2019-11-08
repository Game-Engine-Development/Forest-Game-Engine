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
    explicit Mesh(const char* filename);
    ~Mesh();

    void bindVAO();

    void unbindVAO();

    unsigned int getNumOfVertices();

private:
    unsigned int VAO, VBO, texCoordBuffer, normalBuffer, tangentBuffer, bitangentBuffer, numOfVertices;

    void loadOBJ(const char* filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& textureCoords, std::vector<glm::vec3>& normals);


    void CalculateTangentsAndBitangents(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals,
                      std::vector<glm::vec3> &tangents, std::vector<glm::vec3> &bitangents);
};


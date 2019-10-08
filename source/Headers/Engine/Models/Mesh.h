//
// Created by aekstrand7207 on 10/2/2019.
//

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <vector>


class Mesh {
public:
    Mesh();
    explicit Mesh(const char* filename);
    explicit Mesh(std::vector<glm::vec3>& verticies, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords);
    ~Mesh();

    void bindVAO();

    void unbindVAO();

    unsigned int getNumOfVertices();

private:
    unsigned int VAO, VBO, texCoordBuffer, normalBuffer, numOfVertices;

    void loadOBJ(const char* filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& textureCoords, std::vector<glm::vec3>& normals);
};


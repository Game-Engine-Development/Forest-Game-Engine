#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <vector>


class Mesh {
public:
    Mesh();
    explicit Mesh(const char* filename);
    ~Mesh();

    void bindVAO();

    void unbindVAO();

    unsigned int getNumOfVertices();

private:
    unsigned int VAO, VBO, texCoordBuffer, normalBuffer, numOfVertices;

    void loadOBJ(const char* filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& textureCoords, std::vector<glm::vec3>& normals);
};


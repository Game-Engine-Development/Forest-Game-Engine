#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class TerrainMesh {
public:
    TerrainMesh();
    explicit TerrainMesh(const char* filename);
    ~TerrainMesh();

    void bindVAO();

    static void unbindVAO();

    unsigned int getNumOfVertices();

    static constexpr float SIZE = 1600;
    static constexpr float MAX_HEIGHT = 80;
    float getWidth();
    float getHeight(float x, float z);
    glm::vec3 calculateNormal(float x, float z);
private:
    unsigned char *data = nullptr; //@todo find where data heap memory is freed. I suspect this is a heap memory leak.
    int height;
    void loadTerrain(std::vector<glm::vec3>& verticies, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<unsigned int>& indices, const char* filename);
    unsigned int VAO, VBO, EBO, texCoordBuffer, normalBuffer, numOfVertices; //be careful with making numOfVertices unsigned. unsigned and signed number can NOT mix. //@todo decide whether it is ok to have numOfVertices be unsigned
};
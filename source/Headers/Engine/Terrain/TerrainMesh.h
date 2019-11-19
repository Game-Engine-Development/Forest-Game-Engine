#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class TerrainMesh {
public:

    TerrainMesh();
    explicit TerrainMesh(int type, const char* filename);
    ~TerrainMesh();

    void bindVAO();

    void unbindVAO();

    unsigned int getNumOfVertices();

    static constexpr float SIZE = 1600;
    static constexpr float MAX_HEIGHT = 80;
    float getWidth();
    float getHeight(float x, float z);
    glm::vec3 calculateNormal(float x, float z);
private:
    unsigned char *data;
    int height;
    void loadTerrain(std::vector<glm::vec3>& verticies, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<unsigned int>& indices, const char*);
    unsigned int VAO, VBO, EBO, texCoordBuffer, normalBuffer, numOfVertices;
};
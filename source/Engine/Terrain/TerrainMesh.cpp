//
// Created by Alan on 10/8/2019.
//

#include <iostream>
#include "Headers/stb_image.h"
#include "Headers/Engine/Terrain/TerrainMesh.h"

TerrainMesh::TerrainMesh() = default;

TerrainMesh::TerrainMesh(int type, const char* filename) {
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    loadTerrain(vertices, normals, texCoords, indices, filename);
    numOfVertices = indices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &texCoordBuffer);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &normalBuffer);
    bindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unbindVAO();
}


TerrainMesh::~TerrainMesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &texCoordBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void TerrainMesh::bindVAO() {
    glBindVertexArray(VAO);
}

void TerrainMesh::unbindVAO() {
    glBindVertexArray(0);
}

unsigned int TerrainMesh::getNumOfVertices() {
    return numOfVertices;
}

void TerrainMesh::loadTerrain(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &texCoords, std::vector<unsigned int> &indices, const char* filename) {
    int width, nrchannels;
    data = stbi_load(filename, &width, &height, &nrchannels, 1);
    float VERTEX_COUNT = height;
    for(int i = 0; i < VERTEX_COUNT; ++i){
        for(int j = 0; j < VERTEX_COUNT; ++j){
            vertices.push_back(glm::vec3((float)j/(float)(VERTEX_COUNT - 1) * SIZE, getHeight(j, i), (float)i/(float)(VERTEX_COUNT - 1) * SIZE));
            normals.push_back(calculateNormal(j, i));
            texCoords.push_back(glm::vec2((float)j/(float)(VERTEX_COUNT - 1), (float)i/(float)(VERTEX_COUNT - 1)));
        }
    }
    for(int gz = 0; gz < VERTEX_COUNT - 1; ++gz){
        for(int gx = 0; gx < VERTEX_COUNT - 1; ++gx){
            int topLeft = (gz*VERTEX_COUNT) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz+1)*VERTEX_COUNT) + gx;
            int bottomRight = bottomLeft + 1;
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

float TerrainMesh::getHeight(float x, float z) {
    if(x < 0 || x >= 256 || z < 0 || z >= 256){
        return 0;
    }
    float terrainHeight = data[(int)(z * height + x)];
    terrainHeight /= 127.5;
    terrainHeight -= 1;
    terrainHeight *= MAX_HEIGHT;
    return terrainHeight;
}

glm::vec3 TerrainMesh::calculateNormal(float x, float z) {
    float heightL = getHeight(x - 1, z);
    float heightR = getHeight(x + 1, z);
    float heightU = getHeight(x, z + 1);
    float heightD = getHeight(x, z - 1);
    glm::vec3 normal(heightL - heightR, 2, heightD - heightU);
    normal = glm::normalize(normal);
    return normal;
}
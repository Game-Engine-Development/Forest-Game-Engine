//
// Created by Alan on 10/8/2019.
//

#include "Headers/Engine/Terrain/TerrainMesh.h"

TerrainMesh::TerrainMesh() = default;

TerrainMesh::TerrainMesh(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &texCoords, std::vector<unsigned int> &indices) {
    numOfVertices = indices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &texCoordBuffer);
    glGenBuffers(1, &EBO);
    //glGenBuffers(1, &normalBuffer);
    bindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    //glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    //glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //glEnableVertexAttribArray(2);
    unbindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
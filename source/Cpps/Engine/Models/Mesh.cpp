#include "Headers/Engine/Models/Mesh.h"

Mesh::Mesh() {
    numOfVertices = 0;
}

Mesh::Mesh(const char* filename, bool isNormalMapped) {
    std::vector<glm::vec3> normals, tangents, bitangents;
    std::vector<glm::vec2> textureCoords;
    loadOBJ(filename, vertices, textureCoords, normals);
    numOfVertices = vertices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &texCoordBuffer);
    glGenBuffers(1, &normalBuffer);
    bindVAO();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), &textureCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    if(isNormalMapped) {
        CalculateTangentsAndBitangents(vertices, textureCoords, normals, tangents, bitangents);

        glGenBuffers(1, &tangentBuffer);
        glGenBuffers(1, &bitangentBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
        glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
        glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
    }

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    unbindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Mesh::loadOBJ(const char* filename, std::vector<glm::vec3>& finalVertices, std::vector<glm::vec2>& finalTextureCoords, std::vector<glm::vec3>& finalNormals) {
    FILE *file = std::fopen(filename, "r");
    if(file != nullptr) {
        std::vector<glm::vec3> vertices, normals, final;
        std::vector<glm::vec2> textureCoords;
        std::vector<unsigned int> vertexIndices, textureIndices, normalIndices;
        while (true){
            char lineHeader[128];
            int res = std::fscanf(file, "%s", lineHeader);
            if(res == EOF){
                break;
            }
            if(std::strcmp(lineHeader, "v") == 0) {
                glm::vec3 newVertex;
                std::fscanf(file, "%f %f %f\n", &newVertex.x, &newVertex.y, &newVertex.z);
                vertices.push_back(newVertex);
            } else if(std::strcmp(lineHeader, "vt") == 0) {
                glm::vec2 newTextureCoord;
                std::fscanf(file, "%f %f\n", &newTextureCoord.x, &newTextureCoord.y);
                textureCoords.push_back(newTextureCoord);
            } else if(std::strcmp(lineHeader, "vn") == 0) {
                glm::vec3 newNormal;
                std::fscanf(file, "%f %f %f\n", &newNormal.x, &newNormal.y, &newNormal.z);
                normals.push_back(newNormal);
            } else if(std::strcmp(lineHeader, "f") == 0) {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], textureIndex[3], normalIndex[3];
                int matches = std::fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &textureIndex[0], &normalIndex[0], &vertexIndex[1], &textureIndex[1], &normalIndex[1], &vertexIndex[2], &textureIndex[2], &normalIndex[2]);
                if(matches != 9){
                    std::cerr << "Failed to load Model data at: " << filename << std::endl;
                    break;
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                textureIndices.push_back(textureIndex[0]);
                textureIndices.push_back(textureIndex[1]);
                textureIndices.push_back(textureIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
        }
        for(unsigned int i = 0; i < vertexIndices.size(); ++i) {
            unsigned int vertexIndex = vertexIndices[i];
            glm::vec3 vertex = vertices[vertexIndex - 1];  //obj files are indexed starting at 1 not 0
            finalVertices.push_back(vertex);

            unsigned int textureIndex = textureIndices[i];
            glm::vec2 textureCoord = textureCoords[textureIndex - 1];
            finalTextureCoords.push_back(textureCoord);

            unsigned int normalIndex = normalIndices[i];
            glm::vec3 normal = normals[normalIndex - 1];
            finalNormals.push_back(normal);
        }
    } else {
        std::cerr << "Failed to open " << filename << " model file!" << std::endl;
    }
}

void Mesh::CalculateTangentsAndBitangents(
        // inputs
        std::vector<glm::vec3> & vertices,
        std::vector<glm::vec2> & uvs,
        std::vector<glm::vec3> & normals,
        // outputs
        std::vector<glm::vec3> & tangents,
        std::vector<glm::vec3> & bitangents) {

    for (int i = 0; i < vertices.size(); i += 3) {

        // Shortcuts for vertices
        glm::vec3 &v0 = vertices[i + 0];
        glm::vec3 &v1 = vertices[i + 1];
        glm::vec3 &v2 = vertices[i + 2];

        // Shortcuts for UVs
        glm::vec2 &uv0 = uvs[i + 0];
        glm::vec2 &uv1 = uvs[i + 1];
        glm::vec2 &uv2 = uvs[i + 2];

        // Edges of the triangle : position delta
        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        // Set the same tangent for all three vertices of the triangle.
        // They will be merged later, in vboindexer.cpp
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        // Same thing for bitangents
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }
}


Mesh::~Mesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &texCoordBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &tangentBuffer);
    glDeleteBuffers(1, &bitangentBuffer);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::bindVAO() {
    glBindVertexArray(VAO);
}

void Mesh::unbindVAO() {
    glBindVertexArray(0);
}

unsigned int Mesh::getNumOfVertices() {
    return numOfVertices;
}

std::vector<glm::vec3>& Mesh::getVertices() {
    return vertices;
}
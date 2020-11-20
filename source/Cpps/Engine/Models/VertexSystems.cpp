#include "Headers/Engine/Models/VertexSystems.h"


void CalculateTangentsAndBitangents(
        // inputs
        std::vector<glm::vec3> &vertices,
        std::vector<glm::vec2> &uvs,
        std::vector<glm::vec3> &normals,
        // outputs
        std::vector<glm::vec3> &tangents,
        std::vector<glm::vec3> &bitangents) {

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
        const glm::vec3 deltaPos1 = v1 - v0;
        const glm::vec3 deltaPos2 = v2 - v0;

        // UV delta
        const glm::vec2 deltaUV1 = uv1 - uv0;
        const glm::vec2 deltaUV2 = uv2 - uv0;

        const float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        const glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        const glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

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

void updateCube(glm::vec3 &vertex, const glm::vec3 greatest, const glm::vec3 smallest) {
    for(int i = 0; i < glm::vec3::length(); ++i) {
        vertex[i] = std::clamp(vertex[i], smallest[i], greatest[i]);
    }
}

[[deprecated]]
void loadOBJ(const char *const filename, std::vector<glm::vec3>& finalVertices, std::vector<glm::vec2>& finalTextureCoords, std::vector<glm::vec3>& finalNormals) {
    FilePointerWrapper file(filename, "r"); //if premature exit, FILE* is still guaranteed to be closed
    if(file.getFile() != nullptr) {
        std::vector<glm::vec3> verticesLocal, normalsLocal, final;
        std::vector<glm::vec2> textureCoordsLocal;
        std::vector<unsigned int> vertexIndices, textureIndices, normalIndices;
        while(true) {
            char lineHeader[128];
            const int res = std::fscanf(file.getFile(), "%s", lineHeader);
            if(res == EOF){
                break;
            }
            if(!std::strcmp(lineHeader, "v")) {
                glm::vec3 newVertex;
                std::fscanf(file.getFile(), "%f %f %f\n", &newVertex.x, &newVertex.y, &newVertex.z);
                verticesLocal.push_back(newVertex);
            } else if(!std::strcmp(lineHeader, "vt")) {
                glm::vec2 newTextureCoord;
                std::fscanf(file.getFile(), "%f %f\n", &newTextureCoord.x, &newTextureCoord.y);
                textureCoordsLocal.push_back(newTextureCoord);
            } else if(!std::strcmp(lineHeader, "vn")) {
                glm::vec3 newNormal;
                std::fscanf(file.getFile(), "%f %f %f\n", &newNormal.x, &newNormal.y, &newNormal.z);
                normalsLocal.push_back(newNormal);
            } else if(!std::strcmp(lineHeader, "f")) {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], textureIndex[3], normalIndex[3];
                const int matches = std::fscanf(file.getFile(), "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &textureIndex[0], &normalIndex[0], &vertexIndex[1], &textureIndex[1], &normalIndex[1], &vertexIndex[2], &textureIndex[2], &normalIndex[2]);
                if(matches != 9){
                    std::cerr << "Failed to load Model data at: " << filename << std::endl;
                    break;
                }
                for(int i = 0; i < 3; ++i) {
                    vertexIndices.push_back(vertexIndex[i]);
                    textureIndices.push_back(textureIndex[i]);
                    normalIndices.push_back(normalIndex[i]);
                }
            }
        }
        for(unsigned int i = 0; i < vertexIndices.size(); ++i) {
            const unsigned int vertexIndex = vertexIndices[i];
            glm::vec3 vertex = verticesLocal[vertexIndex - 1];  //obj files are indexed starting at 1 not 0
            finalVertices.push_back(vertex);
            updateCube(vertex);

            const unsigned int textureIndex = textureIndices[i];
            const glm::vec2 textureCoord = textureCoordsLocal[textureIndex - 1];
            finalTextureCoords.push_back(textureCoord);

            const unsigned int normalIndex = normalIndices[i];
            const glm::vec3 normal = normalsLocal[normalIndex - 1];
            finalNormals.push_back(normal);
        }
    } else {
        std::cerr << "Failed to open " << filename << " model file!" << std::endl;
    }
}


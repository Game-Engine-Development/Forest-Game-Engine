#include "Headers/Engine/Models/Mesh.h"

void swap(Mesh &mesh1, Mesh &mesh2) {
    std::swap(mesh1.meshCacheKey, mesh2.meshCacheKey);
    std::swap(mesh1.VAOCache, mesh2.VAOCache);

    std::swap(mesh1.numOfVerticesCache, mesh2.numOfVerticesCache);
    std::swap(mesh1.vertices, mesh2.vertices);
    std::swap(mesh1.normals, mesh2.normals);
    std::swap(mesh1.textureCoords, mesh2.textureCoords);
    std::swap(mesh1.normalMapped, mesh2.normalMapped);
    std::swap(mesh1.greatest, mesh2.greatest);
    std::swap(mesh1.smallest, mesh2.smallest);
}

Mesh::Mesh(const std::string &filename, const bool isNormalMapped)
: meshCacheKey(filename), normalMapped(isNormalMapped)
{
    if(meshCache.count(meshCacheKey)) {
        meshCache.at(meshCacheKey).second += 1;

        pollIsLoaded();
    }
    else {
        addNullEntry();

        std::vector<glm::vec3> tangents, bitangents;
        loadOBJ(filename.c_str(), vertices, textureCoords, normals);

        const std::size_t numOfVertices = vertices.size();

        unsigned int VAO{}, VBO{}, texCoordBuffer{}, normalBuffer{}, tangentBuffer{}, bitangentBuffer{};

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &texCoordBuffer);
        glGenBuffers(1, &normalBuffer);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), &textureCoords[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        if (normalMapped) {
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

        VAOCache = VAO;
        numOfVerticesCache = numOfVertices;
        meshCache.at(meshCacheKey).first = MeshResourceContainer(VAO, VBO, texCoordBuffer, normalBuffer, tangentBuffer, bitangentBuffer, numOfVertices);
    }
}

void Mesh::addNullEntry() {
    meshCache.emplace(std::string(meshCacheKey), std::make_pair<std::optional<MeshResourceContainer>, std::size_t>(std::nullopt, 1));
}

Mesh::Mesh(const Mesh &mesh)
: meshCacheKey(mesh.meshCacheKey), VAOCache(mesh.VAOCache),
  numOfVerticesCache(mesh.numOfVerticesCache), vertices(mesh.vertices),
  normals(mesh.normals), textureCoords(mesh.textureCoords),
  normalMapped(mesh.normalMapped), greatest(mesh.greatest), smallest(mesh.smallest)
{
    meshCache[meshCacheKey].second += 1;
}
Mesh::Mesh(Mesh &&oldMesh) noexcept
: meshCacheKey(std::move(oldMesh.meshCacheKey)), VAOCache(oldMesh.VAOCache),
  numOfVerticesCache(oldMesh.numOfVerticesCache), vertices(std::move(oldMesh.vertices)),
  normals(std::move(oldMesh.normals)), textureCoords(std::move(oldMesh.textureCoords)),
  normalMapped(oldMesh.normalMapped), greatest(oldMesh.greatest), smallest(oldMesh.smallest)
{
    oldMesh.meshCacheKey.clear();
    oldMesh.VAOCache = 0;
    oldMesh.numOfVerticesCache = 0;
    oldMesh.vertices.clear();
    oldMesh.normals.clear();
    oldMesh.textureCoords.clear();
    oldMesh.normalMapped = false;
    oldMesh.greatest = glm::vec3(0.0f);
    oldMesh.smallest = glm::vec3(0.0f);
}

Mesh& Mesh::operator=(const Mesh &mesh) {
    Mesh copy(mesh);
    swap(*this, copy);
    return *this;
}
Mesh& Mesh::operator=(Mesh &&oldMesh) noexcept {
    Mesh move(std::move(oldMesh));
    swap(*this, move);
    return *this;
}

void Mesh::loadOBJ(const char *const filename, std::vector<glm::vec3>& finalVertices, std::vector<glm::vec2>& finalTextureCoords, std::vector<glm::vec3>& finalNormals) {
    FilePointerWrapper file(filename, "r"); //if premature exit, FILE* is still guaranteed to be closed
    if(file.getFile() != nullptr) {
        std::vector<glm::vec3> verticesLocal, normalsLocal, final;
        std::vector<glm::vec2> textureCoordsLocal;
        std::vector<unsigned int> vertexIndices, textureIndices, normalIndices;
        while(true) {
            char lineHeader[128];
            int res = std::fscanf(file.getFile(), "%s", lineHeader);
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
                int matches = std::fscanf(file.getFile(), "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &textureIndex[0], &normalIndex[0], &vertexIndex[1], &textureIndex[1], &normalIndex[1], &vertexIndex[2], &textureIndex[2], &normalIndex[2]);
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
            unsigned int vertexIndex = vertexIndices[i];
            glm::vec3 vertex = verticesLocal[vertexIndex - 1];  //obj files are indexed starting at 1 not 0
            finalVertices.push_back(vertex);
            updateCube(vertex);

            unsigned int textureIndex = textureIndices[i];
            glm::vec2 textureCoord = textureCoordsLocal[textureIndex - 1];
            finalTextureCoords.push_back(textureCoord);

            unsigned int normalIndex = normalIndices[i];
            glm::vec3 normal = normalsLocal[normalIndex - 1];
            finalNormals.push_back(normal);
        }
    } else {
        std::cerr << "Failed to open " << filename << " model file!" << std::endl;
    }
}

void Mesh::CalculateTangentsAndBitangents(
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

void Mesh::bindVAO() {
    pollIsLoaded();
    glBindVertexArray(VAOCache);
}

void Mesh::unbindVAO() {
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if(meshCache.count(meshCacheKey)) {
        meshCache[meshCacheKey].second -= 1;

        //if the refcount drops to 0, delete the element
        if(!meshCache[meshCacheKey].second) meshCache.erase(meshCacheKey);
    }
}

void Mesh::pollIsLoaded() {
    if(!isLoaded && meshCache.at(meshCacheKey).first.has_value()) {
        //std::cout << "Mesh::pollIsLoaded()\n";
        VAOCache = meshCache.at(meshCacheKey).first->getVAO();
        numOfVerticesCache = meshCache.at(meshCacheKey).first->getNumOfVertices();
        isLoaded = true;
        //std::cout << "VAOCache: " << VAOCache << ", numOfVerticesCache: " << numOfVerticesCache << '\n';
    }
}

[[nodiscard]] unsigned int Mesh::getNumOfVertices() {
    pollIsLoaded();
    return numOfVerticesCache;
}
[[nodiscard]] const std::vector<glm::vec3>& Mesh::getVertices() const noexcept {
    return vertices;
}

void Mesh::updateCube(glm::vec3 &vertex) const {
    for(int i = 0; i < glm::vec3::length(); ++i) {
        vertex[i] = std::clamp(vertex[i], smallest[i], greatest[i]);
    }
}

Mesh::FilePointerWrapper::FilePointerWrapper(const char *filename, const char *modes)
: file(std::fopen(filename, modes))
{}

[[nodiscard]] FILE* Mesh::FilePointerWrapper::getFile() const noexcept {
    return file;
}

Mesh::FilePointerWrapper::~FilePointerWrapper() {
    std::fclose(file);
}

#include "Headers/Engine/Scene/Components.h"




Component::MeshComponent::MeshComponent(const std::string &filename, const bool isNormalMapped)
        : meshCacheKey(filename), normalMapped(isNormalMapped)
{
    if(LookupTables::MeshCache.count(meshCacheKey)) {
        LookupTables::MeshCache.at(meshCacheKey).second += 1;

        VAOCache = LookupTables::MeshCache.at(meshCacheKey).first.getVAO();
        numOfVerticesCache = LookupTables::MeshCache.at(meshCacheKey).first.getNumOfVertices();
    }
    else {
        std::vector<glm::vec3> tangents, bitangents;
        std::cout << "loadObj()\n";
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
        LookupTables::MeshCache.emplace(meshCacheKey, std::make_pair(MeshResourceContainer(VAO, VBO, texCoordBuffer, normalBuffer, tangentBuffer, bitangentBuffer, numOfVertices), 1u));
    }
}

Component::MeshComponent::MeshComponent(const Component::MeshComponent &mesh)
        : meshCacheKey(mesh.meshCacheKey), VAOCache(mesh.VAOCache),
          numOfVerticesCache(mesh.numOfVerticesCache), vertices(mesh.vertices),
          normals(mesh.normals), textureCoords(mesh.textureCoords),
          normalMapped(mesh.normalMapped), greatest(mesh.greatest), smallest(mesh.smallest)
{
    LookupTables::MeshCache[meshCacheKey].second += 1;
}
Component::MeshComponent::MeshComponent(Component::MeshComponent && oldMesh) noexcept
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

Component::MeshComponent& Component::MeshComponent::operator=(const MeshComponent &mesh) {
    Component::MeshComponent copy(mesh);
    swap(*this, copy);
    return *this;
}

Component::MeshComponent& Component::MeshComponent::operator=(MeshComponent &&oldMesh) noexcept {
    Component::MeshComponent move(std::move(oldMesh));
    swap(*this, move);
    return *this;
}

void Component::MeshComponent::bindVAO() const {
    glBindVertexArray(VAOCache);
}

void Component::MeshComponent::unbindVAO() {
    glBindVertexArray(0);
}

Component::MeshComponent::~MeshComponent() {
    if(LookupTables::MeshCache.count(meshCacheKey)) {
        LookupTables::MeshCache[meshCacheKey].second -= 1;

        //if the refcount drops to 0, delete the element
        if(!LookupTables::MeshCache[meshCacheKey].second) LookupTables::MeshCache.erase(meshCacheKey);
    }
}

[[nodiscard]] unsigned int Component::MeshComponent::getNumOfVertices() const {
    return numOfVerticesCache;
}
[[nodiscard]] const std::vector<glm::vec3>& Component::MeshComponent::getVertices() const noexcept {
    return vertices;
}


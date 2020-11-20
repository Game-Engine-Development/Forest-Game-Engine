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
    LookupTables::MeshCache.at(meshCacheKey).second += 1;
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
        LookupTables::MeshCache.at(meshCacheKey).second -= 1;

        //if the refcount drops to 0, delete the element
        if(!LookupTables::MeshCache.at(meshCacheKey).second) LookupTables::MeshCache.erase(meshCacheKey);
    }
}

[[nodiscard]] unsigned int Component::MeshComponent::getNumOfVertices() const {
    return numOfVerticesCache;
}
[[nodiscard]] const std::vector<glm::vec3>& Component::MeshComponent::getVertices() const noexcept {
    return vertices;
}



Component::TextureComponent::TextureComponent(const std::string& filename, const int unit, const std::optional<std::string>& nameInShader)
        : textureCacheKey(filename), textureUnit(unit)
{
    shaderName = nameInShader.value_or(std::string("texture") + std::to_string(unit));

    if(LookupTables::TextureCache.count(filename)) {
        LookupTables::TextureCache.at(filename).second += 1;
        IDCache = LookupTables::TextureCache.at(textureCacheKey).first.getID();
    }
    else {
        IDCache = loadOnMain(loadFromDisk(textureCacheKey), textureUnit);
        LookupTables::TextureCache.emplace(std::string(textureCacheKey), std::make_pair<TextureResourceContainer, std::size_t>(
                TextureResourceContainer(IDCache), 1));
    }
}

Component::TextureComponent::TextureComponent(const Component::TextureComponent &tex)
        : textureCacheKey(tex.textureCacheKey), IDCache(tex.IDCache),
          textureUnit(tex.textureUnit), shaderName(tex.shaderName)
{
    LookupTables::TextureCache.at(textureCacheKey).second += 1;
}
Component::TextureComponent::TextureComponent(Component::TextureComponent &&oldTexture) noexcept
        : textureCacheKey(std::move(oldTexture.textureCacheKey)), IDCache(oldTexture.IDCache),
          textureUnit(oldTexture.textureUnit), shaderName(std::move(oldTexture.shaderName))
{
    oldTexture.textureCacheKey.clear();
    oldTexture.IDCache = 0;
    oldTexture.textureUnit = 0;
    oldTexture.shaderName.clear();
}

Component::TextureComponent& Component::TextureComponent::operator=(const Component::TextureComponent &tex) {
    Component::TextureComponent copy(tex);
    swap(*this, copy);
    return *this;
}
Component::TextureComponent& Component::TextureComponent::operator=(Component::TextureComponent &&oldTexture) noexcept {
    Component::TextureComponent move(std::move(oldTexture));
    swap(*this, move);
    return *this;
}

void Component::TextureComponent::bind(const Shader &shader) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, IDCache);
    const int textureLoc = glGetUniformLocation(shader.ID, shaderName.c_str());
    glUniform1i(textureLoc, textureUnit);
}

void Component::TextureComponent::unbind() const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Component::TextureComponent::~TextureComponent() {
    if(LookupTables::TextureCache.count(textureCacheKey)) {
        LookupTables::TextureCache.at(textureCacheKey).second -= 1;

        //if the refcount drops to 0, delete the element
        if(!LookupTables::TextureCache.at(textureCacheKey).second) {
            LookupTables::TextureCache.erase(textureCacheKey);
        }
    }
}


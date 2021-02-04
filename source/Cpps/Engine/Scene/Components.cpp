#include "Headers/Engine/Scene/Components.h"




Component::TextureComponent::TextureComponent(const std::string& filename, const int unit, const aiTextureType type, std::string typeName, const std::optional<std::string>& nameInShader)
        : textureCacheKey(filename), textureUnit(unit), textureType(type), textureTypename(std::move(typeName)), shaderName(nameInShader.value_or(std::string("texture") + std::to_string(unit)))
{
    std::cout << "filename: " << filename << '\n';

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
        : textureCacheKey(tex.textureCacheKey), IDCache(tex.IDCache), textureType(tex.textureType),
          textureUnit(tex.textureUnit), shaderName(tex.shaderName), textureTypename(tex.textureTypename)
{
    LookupTables::TextureCache.at(textureCacheKey).second += 1;
}
Component::TextureComponent::TextureComponent(Component::TextureComponent &&oldTexture) noexcept
        : textureCacheKey(std::move(oldTexture.textureCacheKey)), IDCache(oldTexture.IDCache), textureType(oldTexture.textureType),
          textureUnit(oldTexture.textureUnit), shaderName(std::move(oldTexture.shaderName)), textureTypename(std::move(oldTexture.textureTypename))
{
    oldTexture.textureCacheKey.clear();
    oldTexture.IDCache = 0;
    oldTexture.textureUnit = 0;
    oldTexture.shaderName.clear();
    oldTexture.textureTypename.clear();
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
    const int textureLoc = glGetUniformLocation(*shader.ID, shaderName.c_str());
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







//reference count incremented in the asset loader
Component::Model::Model(std::vector<Mesh> meshes, std::string path)
: meshes(std::move(meshes)), path(std::move(path)) {}

Component::Model::Model(const Model &model)
: meshes(model.meshes), path(model.path)
{
    LookupTables::ModelCache.at(path).second += 1;
}

Component::Model::Model(Model &&oldModel) noexcept
: meshes(std::move(oldModel.meshes)), path(std::move(oldModel.path))
{
    oldModel.meshes.clear();
    oldModel.path.clear();
}

Component::Model& Component::Model::operator=(const Model &model) {
    Model copy(model);
    swap(*this, copy);
    return *this;
}

Component::Model& Component::Model::operator=(Model &&oldModel) noexcept {
    Model move(std::move(oldModel));
    swap(*this, move);
    return *this;
}

Component::Model::~Model() {
    if(LookupTables::ModelCache.count(path)) {
        LookupTables::ModelCache.at(path).second -= 1;

        //if the refcount drops to 0, delete the element
        if(!LookupTables::ModelCache.at(path).second) {
            LookupTables::ModelCache.erase(path);
        }
    }
}


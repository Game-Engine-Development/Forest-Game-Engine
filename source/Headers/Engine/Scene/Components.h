#pragma once

#include <utility>
#include <vector>
#include <string>
#include <utility>
#include <optional>

#include <glm/glm.hpp>

#include "Headers/Engine/Scene/ENTTWrapper.h"
#include "Headers/Engine/Scene/LookUpTables.h"
#include "Headers/Engine/Models/VertexSystems.h"
#include "Headers/Engine/Texture/TextureSystems.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"

namespace Component {
    struct TextureComponent {
        TextureComponent(const std::string &filename, int unit, aiTextureType type, std::string typeName,
                         const std::optional<std::string> &nameInShader = std::nullopt);

        TextureComponent(const std::string &filename, int unit,
                         const std::optional<std::string> &nameInShader = std::nullopt)
                         : TextureComponent(filename, unit, {}, {}, nameInShader) {}

        TextureComponent(const TextureComponent &tex);

        TextureComponent(TextureComponent &&oldTexture) noexcept;

        TextureComponent &operator=(const TextureComponent &tex);

        TextureComponent &operator=(TextureComponent &&oldTexture) noexcept;

        void bind(const Shader &shader) const;

        void unbind() const;

        ~TextureComponent();

        std::string textureCacheKey;

        int textureUnit{};
        std::string shaderName;

    private:
        friend void swap(Component::TextureComponent &tex1, Component::TextureComponent &tex2) {
            std::swap(tex1.textureCacheKey, tex2.textureCacheKey);
            std::swap(tex1.IDCache, tex2.IDCache);

            std::swap(tex1.textureUnit, tex2.textureUnit);
            std::swap(tex1.shaderName, tex2.shaderName);
            
            std::swap(tex1.textureType, tex2.textureType);
            std::swap(tex1.textureTypename, tex2.textureTypename);
        }

//        std::string textureCacheKey;

        unsigned int IDCache{}; //denormalized data from textureCache
//        int textureUnit{};
//        std::string shaderName;
        aiTextureType textureType;
        std::string textureTypename;
    };

    struct Mesh {
        unsigned int VAO{}; //just a copy, DO NOT DELETE
        std::size_t numOfVertices{};

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Component::TextureComponent> textures;

        //@todo readd later
        //bool normalMapped;
        //glm::vec3 greatest{}, smallest{};

        void bindVAO() const {
            glBindVertexArray(VAO);
        }

        static void unbindVAO() {
            glBindVertexArray(0);
        }
    };
}

struct ModelMeshesResourceContainer {
    std::vector<MeshResourceContainer> meshIDs;
    std::vector<Component::Mesh> meshes;
};
namespace LookupTables{
    [[deprecated]]
    inline std::unordered_map<std::string, std::pair<ModelMeshesResourceContainer, std::size_t>> ModelCache;
}

namespace Component {
    struct Model {
        Model() = default;
        Model(std::vector<Mesh> meshes, std::string path);
        Model(const Model &model);
        Model(Model &&oldModel) noexcept;
        Model& operator=(const Model &model);
        Model& operator=(Model &&oldModel) noexcept;
        ~Model();

    private:
        friend void swap(Model &model1, Model &model2) {
            std::swap(model1.meshes, model2.meshes);
            std::swap(model1.path, model2.path);
        }

    public:
        std::vector<Mesh> meshes;
        std::string path;
    };

    struct Drawable { //@todo rewrite this
        Drawable(Component::Model model, Shader shader, std::vector<Uniform> uniforms)
        : shader(std::move(shader)), model(std::move(model)), uniforms(std::move(uniforms))
        {}

        Drawable(Component::Model model, const Shader &shader)
                : Drawable(std::move(model), shader, {})
        {}

        Shader shader; //@todo change this

        Component::Model model;

        std::vector<Uniform> uniforms;

        [[nodiscard]] decltype(uniforms.end()->data)& getUniformData(const int index) {
            return uniforms.at(index).data;
        }
        [[nodiscard]] const decltype(uniforms.end()->data)& getUniformData(const int index) const {
            return uniforms.at(index).data;
        }
        [[nodiscard]] decltype(uniforms.end()->name)& getUniformName(const int index) {
            return uniforms.at(index).name;
        }
        [[nodiscard]] const decltype(uniforms.end()->name)& getUniformName(const int index) const {
            return uniforms.at(index).name;
        }


        Transform modelMatrix{};
    };
}

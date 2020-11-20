#pragma once

#include <vector>
#include <string>
#include <utility>
#include <optional>

#include <glm/glm.hpp>

#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Texture/Texture.h"
#include "Headers/Engine/Scene/ENTTWrapper.h"
#include "Headers/Engine/Scene/LookUpTables.h"
#include "Headers/Engine/Models/VertexSystems.h"

namespace Component {
    struct MeshComponent {
        friend void swap(Component::MeshComponent &mesh1, Component::MeshComponent &mesh2) {
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

        explicit MeshComponent(const std::string &filename, bool isNormalMapped);

        MeshComponent(const MeshComponent &mesh);
        MeshComponent(MeshComponent &&oldMesh) noexcept;

        MeshComponent& operator=(const MeshComponent &mesh);
        MeshComponent& operator=(MeshComponent &&oldMesh) noexcept;

        void bindVAO() const;
        static void unbindVAO();

        [[nodiscard]] unsigned int getNumOfVertices() const;
        [[nodiscard]] const std::vector<glm::vec3>& getVertices() const noexcept;

        ~MeshComponent();


    private:
        std::string meshCacheKey;

        unsigned int VAOCache{};
        std::size_t numOfVerticesCache{};

        std::vector<glm::vec3> vertices, normals;
        std::vector<glm::vec2> textureCoords;
        bool normalMapped;
        glm::vec3 greatest{}, smallest{};
    };

    struct Drawable {
        Drawable(Component::MeshComponent mesh, std::vector<Texture> textures) : mesh(std::move(mesh)), textures(std::move(textures))
        {}

        Component::MeshComponent mesh;
        std::vector<Texture> textures;

        Transform modelMatrix{};
    };
}

#pragma once

#include <vector>
#include <string>
#include <optional>

#include <glm/glm.hpp>

#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Texture/Texture.h"
#include "Headers/Engine/Scene/ENTTWrapper.h"

namespace Component {
    struct MeshComponent {
        explicit MeshComponent(const std::string &filename, bool isNormalMapped);

        MeshComponent(const Mesh &mesh);
        MeshComponent(Mesh &&oldMesh) noexcept;

        MeshComponent& operator=(const MeshComponent &mesh);
        MeshComponent& operator=(MeshComponent &&oldMesh) noexcept;

        void bindVAO();
        static void unbindVAO();

        [[nodiscard]] unsigned int getNumOfVertices();
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
        Mesh mesh;
        std::vector<Texture> textures;
        Transform modelMatrix;
    };
}

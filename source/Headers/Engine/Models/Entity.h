#pragma once

#include <utility>
#include <optional>
#include <vector>
#include <string>
#include <array>
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Texture/Texture.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Collisions/Plane.h"
#include "Headers/Engine/Graphics/Materials/Material.h"
#include "Headers/Engine/Graphics/Lighting/PointLight.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"
#include "Headers/Engine/Utils/MiscUtils.h"

class Entity {
    struct Transform {
        glm::vec3 pos{}, rotation{}, scale{};
    };

    Mesh mesh;
    std::vector<Texture> textures;
    Shader shader;
    std::optional<Material> material;
    Transform transform;
    glm::mat4 modelMatrix{};
    std::vector<Plane> planes;
    double verticalOffset = 0;

    void limitRotation() noexcept;
    void moveEntityPlanes(const std::vector<glm::vec3> &vertices) noexcept;

public:
    Entity(
            const std::string &meshFilename,
            bool isNormalMapped,
            const std::vector<std::string> &textureFilenames,
            const std::optional<std::vector<int>> &textureUnits,
            glm::vec3 position,
            glm::vec3 rotation,
            glm::vec3 scale
    ) noexcept;

    Entity(
            const std::string &meshFilename,
            bool isNormalMapped,
            const std::vector<std::string> &textureFilenames,
            glm::vec3 position,
            glm::vec3 rotation,
            glm::vec3 scale
    ) noexcept;

    Entity(
            const std::string &meshFilename,
            const std::vector<std::string> &textureFilenames,
            glm::vec3 position,
            glm::vec3 rotation,
            glm::vec3 scale
    ) noexcept;

    Entity(
            const std::string &meshFilename,
            bool isNormalMapped,
            const std::vector<std::string> &textureFilenames,
            const std::optional<std::vector<int>> &textureUnits
    ) noexcept;

    Entity(
            const std::string &meshFilename,
            bool isNormalMapped,
            const std::vector<std::string> &textureFilenames
    ) noexcept;

    Entity(
            const std::string &meshFilename,
            const std::vector<std::string> &textureFilenames
    ) noexcept;

    Entity(
            const std::string &meshFilename,
            bool isNormalMapped,
            Material material,
            const Shader &shader,
            glm::vec3 position,
            glm::vec3 rotation,
            glm::vec3 scale
    ) noexcept;

    void render(const Camera& camera, const Shader& shader, glm::vec3 lightPos, glm::vec3 lightColor);

    void render(const Camera &camera, const std::vector<PointLight> &pointLights);

    [[nodiscard]] glm::vec3 getScale() const noexcept;
    [[nodiscard]] glm::vec3 getPos() const noexcept;
    [[nodiscard]] const std::vector<Plane>& getPlanes() const noexcept;
    [[nodiscard]] double getVerticalOffset() const noexcept;

    void setPos(glm::vec3 newPos) noexcept;
    void setPosY(float newPosY) noexcept;
    void setRotation(glm::vec3 newRotation) noexcept;
    void rotate(glm::vec3 rotation) noexcept;
    void translate(glm::vec3 translation) noexcept;
};



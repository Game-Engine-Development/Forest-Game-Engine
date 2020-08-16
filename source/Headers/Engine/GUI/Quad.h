#pragma once

#include <optional>
#include <variant>
#include <functional>
#include <array>
#include <utility>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "Headers/Engine/ResourceContainers/QuadResourceContainer.h"
#include "Headers/Engine/Texture/Texture.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Texture/StaticTextRendering/Text.h"

class Quad {
    using isInitialized = bool;
    static inline std::optional<QuadResourceContainer> quadBuffers = std::nullopt;
    unsigned int VAOCache{};

    glm::vec2 position{}, scale{}, offset{};
    std::variant<Text, Texture> texture;
    std::optional<Texture> backgroundTexture;
    bool hasBackgroundImg{};
    bool isTransparent{};

    void bindVAO() const noexcept;
    static void unbindVAO() noexcept;
    void createBuffers() noexcept;

    static constexpr std::array<glm::vec2, 4> vertices = {
            glm::vec2(0.5f, 0.5f),
            glm::vec2(0.5f, -0.5f),
            glm::vec2(-0.5f, 0.5f),
            glm::vec2(-0.5f, -0.5f)
    };

    static constexpr std::array<glm::vec2, 4> textureCoords = {
            glm::vec2(0, 0),
            glm::vec2(0, 1),
            glm::vec2(1, 0),
            glm::vec2(1, 1)
    };

    static constexpr std::array<unsigned int, 6> indices = {
            0, 1, 2, 1, 3, 2
    };

public:
    explicit Quad(
            std::variant<Text, Texture> textureParam,
            const glm::vec2 &position,
            const glm::vec2 &scale,
            bool isTransparentPar,
            bool hasBackgroundImg,
            std::optional<Texture> backgroundImg
    ) noexcept;

    void render(const Shader& shader);

    [[nodiscard]] static std::array<glm::vec2, 4> getVertices() noexcept;

    [[nodiscard]] glm::vec2 getScale() const noexcept;
    [[nodiscard]] glm::vec2 getPos() const noexcept;
    [[nodiscard]] glm::vec2 getOffset() const noexcept;

    void setOffset(glm::vec2 newOffset) noexcept;
};

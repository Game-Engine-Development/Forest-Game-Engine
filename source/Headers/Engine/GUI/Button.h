#pragma once

#include <optional>
#include <variant>
#include <ctime>
#include <utility>
#include <array>
#include <algorithm>
#include <functional>

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/IO/Input.h"
#include "Headers/Engine/Texture/Texture.h"
#include "Headers/Engine/GUI/Quad.h"
#include "Headers/Engine/Utils/MiscUtils.h"
#include "Headers/Engine/Texture/StaticTextRendering/Text.h"

class Button {
public:
    Button(
            std::variant<Text, Texture> textureParam,
            glm::vec2 position,
            glm::vec2 scale,
            std::function<void(void)> action,
            const Window *window,
            bool isTransparentPar = false,
            bool hasBackgroundImg = false,
            std::optional<Texture> backgroundImg = std::nullopt
    ) noexcept;

    void render(const Shader& shader) noexcept;

private:
    void onClick() noexcept;

    void clampToScreen();

    void detectEdges() noexcept;

    [[nodiscard]] static glm::vec2 clampSides(glm::vec2 cursorEdge, glm::vec2 offset) noexcept;

    bool pressed = false;
    bool notPressed = true;

    std::array<float, 4> edges{};
    std::array<bool, 4> offscreen{}; //x < -1.0f, x > 1.0f, y < -1.0f, y > 1.0f

    const Window *const window = nullptr;

    std::function<void(void)> action;

    Quad quad;
};


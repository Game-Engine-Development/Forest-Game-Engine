#pragma once

#include <ctime>
#include <utility>
#include <array>
#include <functional>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/IO/Input.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/GUI/Quad.h"
#include "Headers/Engine/Math/MathUtils.h"
#include "Headers/Engine/GUI/StaticTextRendering/Text.h"

class Button {
public:
    Button();
    Button( //@todo make an interface for text and Texture to inherit from so that polymorphic pointers can be used
            Text &&staticText,
            glm::vec2 position,
            glm::vec2 scale,
            std::function<void(void)> action,
            Window *window
    );
    Button(
            const char *textureLocation,
            glm::vec2 position,
            glm::vec2 scale,
            std::function<void(void)> action,
            Window *window
    );
    void render(Shader& shader);

private:
    void onClick();

    void clampToScreen();

    void detectEdges();

    bool pressed = false;
    bool notPressed = true;

    std::array<float, 4> edges{};

    Window *window;

    std::function<void(void)> action;

    Quad quad;
};


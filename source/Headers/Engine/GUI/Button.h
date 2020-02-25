#pragma once

#include <Headers/Engine/IO/Input.h>
#include <functional>
#include <glm/vec2.hpp>
#include <Headers/Engine/Models/Texture.h>
#include <Headers/Engine/Camera/Camera.h>
#include <GLFW/glfw3.h>
#include <Headers/Engine/GUI/Quad.h>
#include <array>
#include "Headers/Engine/Math/MathUtils.h"

class Button {
public:
    Button();
    Button(char *textureLocation, glm::vec2 position, glm::vec2 scale, std::function<void(void)> action, Window *window, std::vector<glm::vec2> &&verts, std::vector<glm::vec2> &&texts, std::vector<unsigned int> &&inds);
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


#pragma once

#include <ctime>
#include <utility>
#include <memory>
#include <array>
#include <functional>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "Headers/Engine/IO/Input.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/GUI/Quad.h"
#include "Headers/Engine/Math/MathUtils.h"

class Button {
public:
    Button();
    Button(const char *textureLocation, glm::vec2 position, glm::vec2 scale, std::function<void(void)> action, Window *window);
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

    std::unique_ptr<Quad> quad;
};


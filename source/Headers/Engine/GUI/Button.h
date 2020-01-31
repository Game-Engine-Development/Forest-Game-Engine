#pragma once

#include <functional>
#include <glm/vec2.hpp>
#include <Headers/Engine/Models/Texture.h>
#include <Headers/Engine/Camera.h>
#include <GLFW/glfw3.h>
#include <Headers/Engine/GUI/Quad.h>
#include <array>

class Button {
public:
    Button();
    Button(char *textureLocation, glm::vec2 position, glm::vec2 scale, std::function<void(void)> action, GLFWwindow* window, std::vector<glm::vec2> &&verts, std::vector<glm::vec2> &&texts, std::vector<unsigned int> &&inds);
    void render(Shader& shader);

private:
    void onClick();

    void clampToScreen();

    void detectEdges();

    bool pressed = false;
    bool notPressed = true;

    std::array<float, 4> edges{};

    GLFWwindow* window;

    std::function<void(void)> action;

    GLFWvidmode *mode;

    Quad quad;
};


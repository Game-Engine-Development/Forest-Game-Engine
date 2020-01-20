#pragma once

#include <glm/vec2.hpp>
#include <Headers/Engine/Models/Texture.h>
#include <Headers/Engine/Camera.h>
#include <GLFW/glfw3.h>

class Button {
public:
    Button();
    Button(char *textureLocation, glm::vec2 position, glm::vec2 scale, void (*action)(), GLFWwindow* window, std::vector<glm::vec2> &&verts, std::vector<glm::vec2> &&texts, std::vector<unsigned int> &&inds);
    void render(Shader& shader);

    ~Button();

private:
    void bindVAO();
    void unbindVAO();
    void createBuffers();

    void onClick();

    void clampToScreen();

    void detectEdges();

    bool pressed = false;
    bool notPressed = true;

    std::array<float, 4> edges{};

    std::vector<glm::vec2> vertices, textureCoords;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, TBO, IBO;

    glm::vec2 position, scale, offset;
    Texture texture;

    GLFWwindow* window;

    void (*action)();
};


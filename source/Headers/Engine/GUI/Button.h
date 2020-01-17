#pragma once

#include <glm/vec2.hpp>
#include <Headers/Engine/Models/Texture.h>
#include <Headers/Engine/Camera.h>

class Button {
public:
    Button();
    Button(char *textureLocation, int type, glm::vec2 position, glm::vec2 scale, glm::vec2 rotation, void (*action)(), std::vector<glm::vec2> &&verts, std::vector<glm::vec2> &&texts, std::vector<unsigned int> &&inds);
    void render(Shader& shader);

    ~Button();

private:
    void bindVAO();
    void unbindVAO();
    void createBuffers();

    void onClick();

    void clampToScreen();

    float xOffset, yOffset;
    std::vector<glm::vec2> vertices, textureCoords;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, TBO, IBO;

    glm::vec2 position, scale, rotation;
    Texture texture;

    void (*action)();
};


#pragma once

#include <glm/vec2.hpp>
#include <Headers/Engine/Models/Texture.h>
#include <Headers/Engine/Camera.h>

class Button {
public:
    Button();
    Button(char *textureLocation, int type, glm::vec2 position, glm::vec2 scale, void (*action)(), std::vector<glm::vec2> verts, std::vector<glm::vec2> texts, std::vector<glm::vec2> inds);
    void render(Camera& camera, Shader& shader);
    void setTexture(char *textureLocation, int type);
    void setTexture(Texture newTexture);
    void setPos(glm::vec2 pos);
    void setScale(glm::vec2 scale);
    void setAction(void (*newAction)());
    ~Button();

private:
    glm::mat4 createModelMatrix();
    void bindVAO();
    void unbindVAO();
    void createBuffers();

    void onClick();

    std::vector<glm::vec2> vertices, textureCoords, indices;
    unsigned int VAO, VBO, texCoordBuffer, IBO;

    glm::mat4 modelMatrix;

    glm::vec2 position, scale, rotation;
    Texture texture;

    void (*action)();
};


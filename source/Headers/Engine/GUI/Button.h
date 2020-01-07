#pragma once

#include <glm/vec2.hpp>
#include <Headers/Engine/Models/Texture.h>
#include <Headers/Engine/Camera.h>

class Button {
public:
    Button();
    Button(char *textureLocation, int type, glm::vec2 position, glm::vec2 scale, void (*action)());
    void render(Camera& camera, Shader& shader);
    void setTexture(char *textureLocation, int type);
    void setTexture(Texture newTexture);
    void setPos(glm::vec2 pos);
    void setScale(glm::vec2 scale);
    void setAction(void (*newAction)());
private:
    glm::vec2 position;
    glm::vec2 scale;
    Texture texture;

    void (*action)();
};


#pragma once

#include <functional>
#include <glm/vec2.hpp>
#include <vector>
#include <utility>
#include <GLFW/glfw3.h>

#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/GUI/StaticTextRendering/Text.h"

class Quad {
public:
    Quad();
    explicit Quad( //@todo make an interface for text and Texture to inherit from so that polymorphic pointers can be used
            Text &&staticTextPar,
            const glm::vec2 &position,
            const glm::vec2 &scale,
            bool isTransparentPar,
            bool hasBackgroundImg,
            Texture& backgroundImg
    );
    explicit Quad(
            Texture&& texturePar,
            const glm::vec2 &position,
            const glm::vec2 &scale,
            bool isTransparentPar,
            bool hasBackgroundImg,
            Texture& backgroundImg
    );
    Quad& operator=(Quad &&quad) noexcept ;

    void render(Shader& shader);

    std::vector<glm::vec2> getVertices();

    glm::vec2 getScale();
    glm::vec2 getPos();
    glm::vec2 getOffset();

    void setOffsetX(float x);
    void setOffsetY(float y);

    ~Quad();
private:
    bool usingText; //@todo remove after making Text/Texture interface
    bool hasBackgroundImg;
    bool isTransparent;

    void bindVAO();
    static void unbindVAO();
    void createBuffers();

    std::vector<glm::vec2> vertices = {
            glm::vec2(0.5f,  0.5f),
            glm::vec2(0.5f, -0.5f),
            glm::vec2(-0.5f,  0.5f),
            glm::vec2(-0.5f, -0.5f)
    };

    std::vector<glm::vec2> textureCoords = {
            glm::vec2(0,  0),
            glm::vec2(0, 1),
            glm::vec2(1,  0),
            glm::vec2(1, 1)
    };

    std::vector<unsigned int> indices = {
            0, 1, 2, 1, 3, 2
    };

    unsigned int VAO, VBO, TBO, IBO;

    glm::vec2 position, scale, offset;

    Text staticText; //@todo make an interface for text and Texture to inherit from so that polymorphic pointers can be used
    Texture texture;
    Texture backgroundTexture;
};

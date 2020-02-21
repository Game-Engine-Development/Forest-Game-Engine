#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Headers/Engine/IO/Window.h"

class HDR {
public:
    HDR();
    explicit HDR(Window &window);
    void bind();
    void render(Shader &entityShader, bool hdr, float exposure);
private:
    unsigned int quadVAO = 0;
    unsigned int quadVBO;

    void renderQuad();

    bool HDROn = false;

    unsigned int hdrFBO;
    unsigned int colorBuffer;
    unsigned int rboDepth;
};
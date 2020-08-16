#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/GUI/Quad.h"

class HDR {
public:
    HDR(const HDR&) = delete;
    HDR(HDR&&) = delete;
    HDR& operator=(const HDR&) = delete;
    HDR& operator=(HDR&&) = delete;

    explicit HDR(Window &window);

    void bind() const;
    void render(Shader &entityShader, float exposure);

    bool getHDRStatus() const;
    void setHDRStatus(bool hdr);

    ~HDR();
private:
    unsigned int quadVAO{};
    unsigned int quadVBO{};

    void renderQuad();

    bool HDROn = true;

    unsigned int hdrFBO{};
    unsigned int colorBuffer{};
    unsigned int rboDepth{};
};
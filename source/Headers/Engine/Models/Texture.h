#pragma once

#include <glad/glad.h>
#include <iostream>

class Texture {
public:
    Texture();
    explicit Texture(const char* filename, int type, int unit);
    ~Texture();
    void bind();
    void unbind();
    static constexpr int PNG = 0;
    static constexpr int JPG = 1;
    unsigned int get_ID();
private:
    unsigned int ID;
    int textureUnit;
};


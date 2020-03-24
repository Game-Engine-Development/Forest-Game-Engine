#pragma once

#include <regex>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <Headers/Engine/Shader/Shader.h>

class Texture {
public:
    Texture();
    explicit Texture(const char* filename, int unit);
    void operator()(Texture &&quad);

    void bind(Shader& shader);
    void unbind();

    static constexpr int PNG = 0;
    static constexpr int JPG = 1;

    unsigned int get_ID();

    ~Texture();
private:
    unsigned int ID;
    int textureUnit;
};


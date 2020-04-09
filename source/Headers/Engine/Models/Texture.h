#pragma once

#include <memory>
#include <regex>
#include <glad/glad.h>
#include <iostream>
#include <string>

#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/GUI/TextureResourceContainer.h"

class Texture {
public:
    Texture();
    explicit Texture(const char *filename, int unit, const char* nameInShader);
    Texture(Texture &&oldTexture) noexcept;
    Texture(const Texture &original);

    Texture& operator=(Texture &&oldTexture) noexcept;
    Texture& operator=(const Texture &original);

    void bind(Shader& shader);
    void unbind();

    static constexpr int PNG = 0;
    static constexpr int JPG = 1;

    unsigned int get_ID();

    ~Texture();
private:
    std::string textureFilename;

    std::shared_ptr<TextureResourceContainer> IDContainer = nullptr;
    int textureUnit;
    const char* shaderName;
};

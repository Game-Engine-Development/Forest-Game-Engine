#pragma once

#ifndef STB_IMAGE_IMPLEMENTATION
#include "../../../../libraries/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <array>
#include <memory>

#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/ResourceContainers/TextureResourceContainer.h"

class CubeMapTexture { //@todo find out if copying should be allowed or not
    std::shared_ptr<TextureResourceContainer> IDContainer;
    int texUnit;

public:
    template <size_t N>
    explicit CubeMapTexture(const std::array<const char*, N> &files, int textureUnit);

    void bind(const Shader& shader) const noexcept;

    void unBind() const noexcept;
};

//template definitions:

template <size_t N>
CubeMapTexture::CubeMapTexture(const std::array<const char*, N> &files, const int textureUnit) : texUnit(textureUnit) {
    unsigned int ID{};

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    unsigned char* data = nullptr;
    int width, height, channels;
    for(int i = 0; i < files.size(); ++i) {
        data = stbi_load(files[i], &width, &height, &channels, STBI_rgb);
        if(data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else {
            std::cerr << "Failed to load cubemap!" << std::endl;
            throw;
        }

        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    IDContainer = std::make_shared<TextureResourceContainer>(ID);
}

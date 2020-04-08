#pragma once

#ifndef STB_IMAGE_IMPLEMENTATION
#include <Headers/Engine/Models/stb_image.h>
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <array>
#include <memory>

#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/GUI/TextureResourceContainer.h"

class CubeMapTexture {
public:
    CubeMapTexture();

    template <unsigned int N>
    explicit CubeMapTexture(std::array<const char*, N>& files, int textureUnit) {
        unsigned int ID;

        texUnit = textureUnit;
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

    ~CubeMapTexture();

    void bind(Shader& shader);

    void unBind();
private:
    std::shared_ptr<TextureResourceContainer> IDContainer = nullptr;
    int texUnit;
};

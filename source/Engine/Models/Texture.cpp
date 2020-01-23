#include <regex>
#include "Headers/stb_image.h"
#include "Headers/Engine/Models/Texture.h"

Texture::Texture() = default;

Texture::Texture(const char* filename, int unit) {
    int type;
    if (std::regex_match(filename, std::regex("(.+)(\\.png)"))) {
        type = PNG;
    }
    else if (std::regex_match(filename, std::regex("(.+)(\\.jpg)"))) {
        type = JPG;
    }
    else {
        std::cerr << "File type not supported!. Please supply a JPG or PNG!" << std::endl;
    }
    textureUnit = unit;
    glGenTextures(1, &ID);
    int width, height, nrchannels;
    unsigned char* data = NULL;
    if (type == PNG) {
        data = stbi_load(filename, &width, &height, &nrchannels, STBI_rgb_alpha);
    } else if(type == JPG) {
        data = stbi_load(filename, &width, &height, &nrchannels, STBI_rgb);
    }
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(data){
        if(type == JPG) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if(type == PNG) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load " << filename << " texture!" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() = default;

unsigned int Texture::get_ID(){
    return ID;
}

void Texture::bind(Shader &shader) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    std::string name("texture");
    name += std::to_string(textureUnit);
    int textureLoc = glGetUniformLocation(shader.ID, name.c_str());
    glUniform1i(textureLoc, textureUnit);
}

void Texture::unbind() {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

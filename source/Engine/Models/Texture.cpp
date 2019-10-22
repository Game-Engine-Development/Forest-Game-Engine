#include "Headers/stb_image.h"
#include "Headers/Engine/Models/Texture.h"

Texture::Texture(){
    ID = 5;
}

Texture::Texture(const char* filename, int type, int unit){
    textureUnit = unit;
    glGenTextures(1, &ID);
    int width, height, nrchannels;
    unsigned char* data;
    if (type == PNG) {
        data = stbi_load(filename, &width, &height, &nrchannels, STBI_rgb_alpha);
    } else if(type == JPG) {
        data = stbi_load(filename, &width, &height, &nrchannels, STBI_rgb);
    }
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
        std::cout << "Failed to load " << filename << " texture!" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() = default;

unsigned int Texture::get_ID(){
    return ID;
}

void Texture::bind() {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

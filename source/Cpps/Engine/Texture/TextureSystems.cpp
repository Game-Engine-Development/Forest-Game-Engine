#include "Headers/Engine/Texture/TextureSystems.h"

FileInfo loadFromDisk(const std::string &textureCacheKey) {
    int width{}, height{}, nrchannels{};
    unsigned char *data = nullptr;

    ImageType type{};
    if (std::regex_match(textureCacheKey, std::regex("(.+)(\\.png)")) ||
        std::regex_match(textureCacheKey, std::regex("(.+)(\\.bmp)")))
    {
        type = ImageType::RGBA_IMG;
        data = stbi_load(textureCacheKey.c_str(), &width, &height, &nrchannels, STBI_rgb_alpha);
    } else if (std::regex_match(textureCacheKey, std::regex("(.+)(\\.jpg)")) ||
               std::regex_match(textureCacheKey, std::regex("(.+)(\\.jpeg)")))
    {
        type = ImageType::RGB_IMG;
        data = stbi_load(textureCacheKey.c_str(), &width, &height, &nrchannels, STBI_rgb);
    } else {
        std::cerr << "Failed to load " << textureCacheKey << " texture!\n";
    }

    return FileInfo { width, height, nrchannels, data, type };
}

unsigned int loadOnMain(const FileInfo &entry, const int textureUnit) {
    unsigned int ID{};

    glGenTextures(1, &ID);

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (entry.data) {
        if (entry.type == ImageType::RGB_IMG) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, entry.width, entry.height, 0, GL_RGB, GL_UNSIGNED_BYTE, entry.data);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, entry.width, entry.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, entry.data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Data passed to loadOnMain is null \n";
    }
    stbi_image_free(entry.data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return ID;
}